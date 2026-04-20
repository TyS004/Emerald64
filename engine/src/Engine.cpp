#include "Engine.h"

bool GCGameEngine::Engine::isRunning = true;

static GCGameEngine::Window* window = new GCGameEngine::Window("Editor", 800, 600);
static GCGameEngine::VBO* vbo = new GCGameEngine::VBO(window->getDevice());

void GCGameEngine::Engine::run(){
    while(isRunning){
        SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(window->getDevice());
        GCGameEngine::Pipeline* pipeline = new GCGameEngine::Pipeline(window->getDevice(), "../assets/shaders/object");

        SDL_GPUTexture* swapchain;
        Uint32 width, height;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window->getWindow(), &swapchain, &width, &height);

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.clear_color = {240/255.0f, 240/255.0f, 240/255.0f, 255/255.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.texture = swapchain;

        GCGameEngine::RenderPass::begin(cmdBuffer, &colorTargetInfo);

        if(!window->update()) 
        {
            break;
        }

        GCGameEngine::RenderPass::end();
        SDL_SubmitGPUCommandBuffer(cmdBuffer);
    }
}