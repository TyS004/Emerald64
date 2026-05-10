#include "Engine.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

bool E64::Engine::running = true;
E64::EngineCtx* E64::Engine::ctx = new E64::EngineCtx();

void E64::Engine::run(){
    E64::Renderer* renderer = new E64::Renderer();
    ctx->renderer = renderer;
    ctx->elapsed  = 0.0f;

    SDL_GPUDevice* device = E64::Window::getDevice();
    SDL_Window* window = E64::Window::getWindow();
    
    Pipeline* pipeline = new Pipeline("../assets/shaders/object");

    while(running){
        auto start = std::chrono::high_resolution_clock::now();

        E64::Window::PollEvent();
        if(!E64::Input::isRunning()) break;

        for(Layer* layer : E64::Layer::layers){
            layer->OnUpdate();
        }   

        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        for(Layer* layer : E64::Layer::layers){
            layer->OnImGuiRender(ctx);
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

        SDL_Delay(10);

        auto end = std::chrono::high_resolution_clock::now();
        ctx->elapsed = (end - start).count();
    }

    E64::Input::clean();
    
    delete pipeline;
    delete ctx->active_scene;
    delete ctx->renderer;
    delete ctx;
}

void E64::Engine::exit(){
    running = false;
}