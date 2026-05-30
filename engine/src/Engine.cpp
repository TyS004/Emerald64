#include "Engine.h"

#include "Layer/Layer.h"
#include "Window/Window.h"

#include <unistd.h>

bool E64::Engine::running = true;
std::unique_ptr<E64::EngineCtx> E64::Engine::ctx = std::make_unique<E64::EngineCtx>();

void E64::Engine::run(){
    E64::IRenderer* renderer = ctx->renderer;

    SDL_GPUDevice* device = E64::Window::getDevice();
    SDL_Window* window = E64::Window::getWindow();

    float dt  = 0.0f;
    while(running){
        auto start = std::chrono::high_resolution_clock::now();

        E64::Window::PollEvent();
        if(!E64::Input::isRunning()) break;

        if(renderer->isPendingResize())
        {
            renderer->ResizeViewport();
        }

        renderer->aquireCmdBufferandSwapChain();

        for(Layer* layer : E64::Layer::layers){
            layer->OnUpdate(dt);
        }
        for(Layer* layer : E64::Layer::layers){
            layer->OnRender();
        }
        for(Layer* layer : E64::Layer::layers){
            layer->OnImGuiRender();
        }
        
        renderer->submit();

        auto end = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(end - start).count();
    }
}

void E64::Engine::exit(){
    running = false;
}