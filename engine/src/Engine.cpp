#include "Engine.h"

bool GCGameEngine::Engine::running = true;

void GCGameEngine::Engine::run(){
    SDL_GPUDevice* device = GCGameEngine::Window::getDevice();
    SDL_Window* window = GCGameEngine::Window::getWindow();

    Pipeline* pipeline = new Pipeline("../assets/shaders/object");
    VBO* vbo = new VBO(device, 3);

    while(running){
        SDL_Event event = GCGameEngine::Window::PollEvent();
        if(!GCGameEngine::EventDispatcher::handle(event)){ break; }

        SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);

        SDL_GPUTexture* swapchain;
        Uint32 width, height;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buffer, window, &swapchain, &width, &height);

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.clear_color = {80/255.0f, 80/255.0f, 80/255.0f, 255/255.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.texture = swapchain;

        GCGameEngine::Renderer::begin(cmd_buffer, &colorTargetInfo);
        GCGameEngine::Renderer::bindPipeline(pipeline->getPipeline());
        GCGameEngine::Renderer::bindVertexBuffers(vbo->getBufferBinding());
        GCGameEngine::Renderer::draw();
        GCGameEngine::Renderer::end();

        SDL_SubmitGPUCommandBuffer(cmd_buffer);
    }
}