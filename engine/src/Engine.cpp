#include "Engine.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

bool E64::Engine::running = true;
std::unique_ptr<E64::EngineCtx> E64::Engine::ctx = std::make_unique<E64::EngineCtx>();

void E64::Engine::run(){
    ctx->renderer = std::make_unique<E64::Renderer>();
    E64::Renderer* renderer = ctx->renderer.get();

    SDL_GPUDevice* device = E64::Window::getDevice();
    SDL_Window* window = E64::Window::getWindow();
    
    std::unique_ptr<Pipeline> pipeline = std::make_unique<Pipeline>("../assets/shaders/object");

    float dt  = 0.0f;
    while(running){
        auto start = std::chrono::high_resolution_clock::now();

        E64::Window::PollEvent();
        if(!E64::Input::isRunning()) break;

        for(Layer* layer : E64::Layer::layers){
            layer->OnUpdate(dt);
        }

        for(Layer* layer : E64::Layer::layers){
            layer->OnImGuiRender();
        }

        renderer->beginSceneRenderPass();
        renderer->bindPipeline(pipeline->getPipeline());
        
        for(Layer* layer : E64::Layer::layers){
            layer->OnRender();
        }
        renderer->endRenderPass();

        renderer->beginUIRenderPass();
        renderer->drawUI();
        renderer->endRenderPass();
        
        renderer->submit();

        SDL_Delay(1);

        auto end = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float>(end - start).count();
    }
}

void E64::Engine::exit(){
    running = false;
}