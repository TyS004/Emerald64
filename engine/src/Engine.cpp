#include "Engine.h"

bool GCGameEngine::Engine::isRunning = true;

static GCGameEngine::Window* window = new GCGameEngine::Window("Editor", 800, 600);

void GCGameEngine::Engine::run(){
    Pipeline* pipeline = new Pipeline(window->getDevice(), "../assets/shaders/object");
    VBO* vbo = new VBO(window->getDevice());
    SDL_GPUBufferBinding* binding = vbo->getBufferBinding();

    while(isRunning){
        if(!window->update()){
            break;
        }

        SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(window->getDevice());

        SDL_GPUTexture* swapchain;
        Uint32 width, height;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window->getWindow(), &swapchain, &width, &height);

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.clear_color = {1.0f, 1.0f, 1.0f, 1.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.texture = swapchain;

        GCGameEngine::Renderer::begin(cmdBuffer, &colorTargetInfo);
        GCGameEngine::Renderer::bindPipeline(pipeline->getPipeline());
        //GCGameEngine::Renderer::bindVertexBuffers(binding);
        GCGameEngine::Renderer::draw();
        GCGameEngine::Renderer::end();

        SDL_SubmitGPUCommandBuffer(cmdBuffer);
    }
    delete window;
}