#include "Layer/UILayer.h"
#include "UIInput/UIInput.h"
#include <E64.h>

using namespace E64;

Editor::UILayer::UILayer(){
    E64::Scene* scene = new E64::Scene();
    Editor::EditorCamera* camera = Editor::UIInput::getCamera();
    scene->setCameraData({camera->getProj(), camera->getView()});
    E64::Engine::ctx->active_scene = scene;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    SDL_Window* window = E64::Window::getWindow();
    SDL_GPUDevice* device = E64::Window::getDevice();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLGPU(E64::Window::getWindow());
    ImGui_ImplSDLGPU3_InitInfo init_info = {};
    init_info.Device = E64::Window::getDevice();
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // Only used in multi-viewports mode.
    init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
    init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    ImGui_ImplSDLGPU3_Init(&init_info);

    E64::Input::OnKeyPressedBind = Editor::UIInput::OnKeyPressed;
    E64::Input::OnKeyDownBind = Editor::UIInput::OnKeyDown;
    E64::Input::OnMouseMoveBind = Editor::UIInput::OnMouseMove;
}

Editor::UILayer::~UILayer(){

}

void Editor::UILayer::OnEvent(SDL_Event* e){
    ImGui_ImplSDL3_ProcessEvent(e);
}

void Editor::UILayer::OnUpdate(){
    Editor::UIInput::getCamera()->OnUpdate();
}

void Editor::UILayer::OnImGuiRender(E64::EngineCtx* ctx){
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);

    ImVec2 cur_viewport_size = ImGui::GetContentRegionAvail();
    
    if (cur_viewport_size.x != viewport_size.x || cur_viewport_size.y != viewport_size.y) {
        viewport_size = cur_viewport_size;
        Editor::UIInput::OnWindowResize(viewport_size.x, viewport_size.y);
        Editor::UIInput::getCamera()->OnResize(viewport_size.x, viewport_size.y);
    }

    ImGui::Image((ImTextureID)ctx->renderer->getSceneTexture(), ImGui::GetContentRegionAvail());

    double ms = ctx->elapsed;
    double FPS = (1 / ms) * 1000;

    ImGui::End();

    ImGui::Begin("ECS");
    int x = 0;
    ImGui::Selectable("Entity", &x);
    ImGui::End();
}