#include "Layer/EditorLayer.h"
#include "EditorInput/EditorInput.h"
#include <E64.h>

using namespace E64;

Editor::EditorLayer::EditorLayer(){
    Editor::EditorInput::Init();
    
    E64::Scene* scene = new E64::Scene();
    Editor::EditorCamera* camera = Editor::EditorInput::getCamera();
    scene->setCameraData({camera->getProj(), camera->getView()});
    E64::Engine::ctx->active_scene = scene;

    selected = -1;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    SDL_Window* window = E64::Window::getWindow();
    SDL_GPUDevice* device = E64::Window::getDevice();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLGPU(E64::Window::getWindow());
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = E64::Window::getDevice();
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // multi-viewports mode
    init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // multi-viewports mode
    init_info.PresentMode = SDL_GPU_PRESENTMODE_IMMEDIATE;
    ImGui_ImplSDLGPU3_Init(&init_info);
}

Editor::EditorLayer::~EditorLayer(){

}

void Editor::EditorLayer::OnEvent(SDL_Event* e){
    ImGui_ImplSDL3_ProcessEvent(e);
}

void Editor::EditorLayer::OnUpdate(float dt){
    Editor::EditorInput::getCamera()->OnUpdate(dt);

    FPS = (1.0f / dt);
    ms = dt * 1000;
}

void Editor::EditorLayer::OnImGuiRender(){
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;

    buildDockspace();
    ImGui::ShowDemoWindow();
    buildViewport();
    buildSceneSelector();
    buildInspector();
    buildFileManager();
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
    if(E64::Window::isMouseLock()){
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::Begin("Viewport", nullptr);

    ImVec2 cur_viewport_size = ImGui::GetContentRegionAvail();
    ImVec2 viewport_tl = ImGui::GetCursorScreenPos();
    
    if (cur_viewport_size.x != viewport_size.x || cur_viewport_size.y != viewport_size.y) {
        viewport_size = cur_viewport_size;
        Editor::EditorInput::OnWindowResize(viewport_size.x, viewport_size.y);
        Editor::EditorInput::getCamera()->OnResize(viewport_size.x, viewport_size.y);
    }
    ImGui::Image((ImTextureID)E64::Engine::ctx->renderer->getSceneTexture(), ImGui::GetContentRegionAvail());
    if(Editor::EditorInput::debug_mode) buildDebug(viewport_tl);

    ImGui::End();
}

void Editor::EditorLayer::buildDebug(ImVec2 viewport_tl){
    std::string FPS_UI = "FPS: " + std::to_string(FPS);
    std::string MS_UI = "ms: " + std::to_string(ms);
    std::string VSYNC_UI = "VSync: " + (E64::Window::getVSync() ? std::string("ON") : std::string("OFF"));
    std::string draw_calls = "E64::Renderer::DrawIndexedPrimitves(): " + std::to_string(E64::Engine::ctx->renderer->getDrawCalls());

    ImGui::GetWindowDrawList()->AddText(viewport_tl, IM_COL32(255, 255, 255, 255), FPS_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad), IM_COL32(255, 255, 255, 255), MS_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad * 2), IM_COL32(255, 255, 255, 255), VSYNC_UI.c_str());
    ImGui::GetWindowDrawList()->AddText(ImVec2(viewport_tl.x, viewport_tl.y + debug_pad * 3), IM_COL32(255, 255, 255, 255), draw_calls.c_str());
}

void Editor::EditorLayer::buildSceneSelector(){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    if(E64::Window::isMouseLock()){
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::Begin("Scene");
    for(ECS::Entity e : E64::Engine::ctx->active_scene->getEntites()){
        ImGui::PushID(e);   
        bool is_selected = (selected == e);
        if(ImGui::Selectable(std::string("Entity: " + std::to_string(e)).c_str(), &is_selected)){
            selected = e;
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

    ECS::ComponetMask mask = ECS::EntityManager::entity_index[selected];
    if(mask & ECS::TRANSFORM) buildTransformHeader();
    if(mask & ECS::MESH)      buildMeshHeader();

    ImGui::End();
}   

void Editor::EditorLayer::buildTransformHeader(){
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if(ImGui::CollapsingHeader("Transform")){
        ECS::TransformComponet* transform = ECS::ComponetManager::getComponet<ECS::TransformComponet>(selected);
        ImGui::DragFloat3("Position", &transform->position.x);
    }
}

void Editor::EditorLayer::buildMeshHeader(){
    uint32_t mesh_id = ECS::ComponetManager::getComponet<ECS::MeshComponet>(selected)->id;
    ECS::Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_id);

    ImGui::CollapsingHeader("Mesh");
    ImGui::Text("ID: %d", mesh_id);
    std::string name = mesh->path;
    ImGui::Text("Name: %s", name.c_str());
}

void Editor::EditorLayer::buildFileManager(){
    ImGui::Begin("Files");

    int i = 0;
    for(const ECS::Mesh& mesh : E64::Engine::ctx->asset_manager->getMeshes()){
        ImGui::PushID(i);
        ImGui::Selectable("Asset");
        ImGui::PopID();
        i++;
    }

    ImGui::End();
}