#include "EditorLayer/EditorLayer.h"
#include "EditorInput/EditorInput.h"

#include "AssetImporter/AssetImporter.h"
#include "Serialization/MeshSerializer.h"

#include <Renderer/SDLRenderer.h>

#include <filesystem>
#include <E64.h>

using namespace E64;

Editor::EditorLayer::EditorLayer(){
    input = dynamic_cast<Editor::EditorInput*>(E64::Engine::ctx->input);
    camera = new Editor::EditorCamera();

    selected = 0;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    sdl_window = dynamic_cast<E64::SDLWindow*>(E64::Engine::ctx->window);

    SDL_Window* window = sdl_window->getWindow();
    SDL_GPUDevice* device = sdl_window->getDevice();
    if(!window) E64::Log::error("EDITORLAYER: NO WINDOW FOUND!");
    if(!device) E64::Log::error("EDITORLAYER: NO DEVICE FOUND!");

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLGPU(window);
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = device;
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // multi-viewports mode
    init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // multi-viewports mode
    init_info.PresentMode = SDL_GPU_PRESENTMODE_IMMEDIATE;
    ImGui_ImplSDLGPU3_Init(&init_info);

    float scale = SDL_GetWindowDisplayScale(window);

    ImFontConfig cfg;
    cfg.SizePixels = 18.0f * scale;
    cfg.OversampleH = 4;
    cfg.OversampleV = 4;
    cfg.PixelSnapH = true;

    io.ConfigWindowsMoveFromTitleBarOnly = true;
    
    font = io.Fonts->AddFontFromFileTTF("../assets/fonts/Inter-Regular.ttf", 18.0f * scale, &cfg);
    io.FontGlobalScale = 1.0f / scale;
}

Editor::EditorLayer::~EditorLayer(){

}

void Editor::EditorLayer::OnAttach(){
    E64::Scene* scene = Engine::ctx->active_scene.get();
    ECS::CameraData camera_comp = { camera->getProj(), camera->getView() };
    scene->setCameraData(camera_comp);

    std::string path = "/Users/tylerstier/Desktop/Emerald64/assets/meshes/";
    AssetImporter importer;
    MeshSerializer serializer;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if(entry.path().extension() == ".obj")
        {
            ECS::Mesh mesh = importer.importMesh(entry.path().string());
            std::cout << "PATH: " << mesh.obj_path << std::endl;
            E64::Engine::ctx->asset_manager->addMesh(mesh);

            std::cout << "PATH: " << mesh.obj_path << std::endl;
            serializer.serialize(&mesh);
        }
    }
}

void Editor::EditorLayer::OnUpdate(float dt){
    camera->OnUpdate(dt);

    FPS = (1.0f / dt);
    ms = dt * 1000;
}

void Editor::EditorLayer::OnImGuiRender(){
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    initStyle();

    buildMainMenuBar();
    buildDockspace();
    buildViewport();
    buildSceneSelector();
    buildInspector();
    buildFileManager();
    
    ImGui::Render();

    SDLRenderer* renderer = static_cast<SDLRenderer*>(E64::Engine::ctx->renderer);
    ImGui_ImplSDLGPU3_PrepareDrawData(ImGui::GetDrawData(), renderer->getCommandBuffer());
    renderer->beginRenderPass(SWAPCHAIN);
    renderer->drawUI();
    renderer->endRenderPass();
}

void Editor::EditorLayer::initStyle(){
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(5.0f, 3.0f);
    style.CellPadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
    style.ScrollbarSize = 13.0f;
    style.GrabMinSize = 10.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;

    style.WindowRounding = 4.0f;
    style.ChildRounding = 3.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
}

void Editor::EditorLayer::buildMainMenuBar(){
    if(ImGui::BeginMainMenuBar()){
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Save Scene", "(CTRL + S)")){
                E64::SceneSerializer serializer;
                serializer.serialize();
            }
            if(ImGui::MenuItem("Load Scene")){
                E64::SceneSerializer serializer;
                E64::Scene* scene = serializer.deserialize();
                E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(*scene));
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor::EditorLayer::buildDockspace(){
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);      
    ImGui::Begin("DockSpace", nullptr,
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id);
    ImGui::PopStyleVar(2);
    ImGui::End();
}

void Editor::EditorLayer::buildViewport(){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    if(sdl_window->isMouseLock()){
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::Begin("Viewport", nullptr);

    ImVec2 cur_viewport_size = ImGui::GetContentRegionAvail();
    ImVec2 viewport_tl = ImGui::GetCursorScreenPos();
    
    if (cur_viewport_size.x != viewport_size.x || cur_viewport_size.y != viewport_size.y) {
        viewport_size = cur_viewport_size;
        input->OnWindowResize(viewport_size.x, viewport_size.y);
        camera->OnResize(viewport_size.x, viewport_size.y);
    }
    SDLRenderer* renderer = static_cast<SDLRenderer*>(E64::Engine::ctx->renderer);
    ImGui::Image((ImTextureID)renderer->getSceneTexture(), ImGui::GetContentRegionAvail());
    if(input->debug_mode) buildDebug(viewport_tl);

    ImGui::End();
}

void Editor::EditorLayer::buildDebug(ImVec2 viewport_tl){
    std::string FPS_UI = "FPS: " + std::to_string(FPS);
    std::string MS_UI = "ms: " + std::to_string(ms);
    std::string VSYNC_UI = "VSync: " + (sdl_window->getVSync() ? std::string("ON") : std::string("OFF"));
    std::string draw_calls = "E64::Renderer::DrawIndexedPrimitves(): " + std::to_string(E64::Engine::ctx->renderer->getDrawCalls());
    
    ImGui::GetWindowDrawList()->AddText(viewport_tl, IM_COL32(255, 255, 255, 255), FPS_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad),     IM_COL32(255, 255, 255, 255), MS_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad * 2), IM_COL32(255, 255, 255, 255), VSYNC_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad * 3), IM_COL32(255, 255, 255, 255), draw_calls.c_str());
}

void Editor::EditorLayer::buildSceneSelector(){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    if(sdl_window->isMouseLock()){
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::Begin("Scene");
    for(ECS::Entity e : E64::Engine::ctx->active_scene->getEntites()){
        ImGui::PushID(e);   
        bool is_selected = (selected == e);
        if(ImGui::Selectable(std::string("Entity: " + std::to_string(e)).c_str(), &is_selected)){
            selected = e;
            input->selected_entity = selected;
        }
        ImGui::PopID();

        if (is_selected) {
            ImGui::SetItemDefaultFocus();
        }
    }
    ImGui::End();
}

void Editor::EditorLayer::buildInspector(){
    char name[50];
    int id = selected;

    ImGui::Begin("Inspector");

    ImGui::InputText("Name", name, 50);
    ImGui::InputInt("ID", &selected);

    if(ECS::ComponentManager::hasComponent<ECS::TransformComponent>(selected)) buildTransformHeader();
    if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(selected))      buildMeshHeader();
    if(ECS::ComponentManager::hasComponent<ECS::CameraComponent>(selected))    buildCameraHeader();

    ImGui::End();
}   

void Editor::EditorLayer::buildTransformHeader(){
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if(ImGui::CollapsingHeader("Transform")){
        ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(selected);
        ImGui::DragFloat3("Position", &transform->position.x, 0.05f);
        ImGui::DragFloat3("Rotation", &transform->euler.x, 0.01f);
        ImGui::DragFloat3("Scale", &transform->scale.x, 0.05f);

        ImGui::PushID(0);
        if(ImGui::SmallButton("-"))
        {
            ECS::ComponentManager::removeComponent<ECS::TransformComponent>(selected);
        }
        if(ImGui::Button("+", ImVec2{ImGui::GetContentRegionAvail().x, 50.0f})){
            ECS::ComponentManager::addComponent<ECS::TransformComponent>(selected);
        }
        ImGui::PopID();
    }
}

void Editor::EditorLayer::buildMeshHeader(){
    ECS::MeshComponent* mesh = ECS::ComponentManager::getComponent<ECS::MeshComponent>(selected);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if(ImGui::CollapsingHeader("Mesh"))
    {
        ImGui::Text("Mesh ID: %d", mesh->mesh_handle.id);

        std::string name = mesh->mesh_handle.path;
        ImGui::Text("Name: %s", name.c_str());

        ImGui::Text("Texture");
        if(ImGui::BeginDragDropTarget()){
            ImGui::EndDragDropTarget();
        }

        ImGui::PushID(1);
        if(ImGui::SmallButton("-"))
        {
            ECS::ComponentManager::removeComponent<ECS::MeshComponent>(selected);
        }
        if(ImGui::Button("+", ImVec2{ImGui::GetContentRegionAvail().x, 50.0f})){
            ECS::ComponentManager::addComponent<ECS::MeshComponent>(selected);
        }
        ImGui::PopID();
    }
}

void Editor::EditorLayer::buildCameraHeader(){
    ECS::CameraComponent* camera = ECS::ComponentManager::getComponent<ECS::CameraComponent>(selected);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Text("FOV: %f", camera->fov);
        ImGui::Text("Aspect Ratio: %f", camera->aspect_ratio);
        ImGui::Text("Near Plane: %f", camera->near_plane);
        ImGui::Text("Far Plane: %f", camera->far_plane);
    }
}

void Editor::EditorLayer::buildFileManager(){
    ImGui::Begin("Files");

    int i = 0;
    for(const ECS::Mesh* mesh : E64::Engine::ctx->asset_manager->getMeshes()){
        std::filesystem::path path = E64::Engine::ctx->asset_manager->getMeshes()[i]->obj_path;
        ImGui::PushID(i);
        ImGui::Selectable(path.c_str());
        if(ImGui::BeginDragDropSource()){
            ImGui::SetDragDropPayload("Mesh ID", &i, sizeof(int));
            ImGui::EndDragDropSource();
        }
        ImGui::PopID();
        i++;
    }

    ImGui::End();
}