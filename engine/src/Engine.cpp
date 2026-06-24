#include "Engine.h"

#include "Layer/Layer.h"

bool E64::Engine::running = true;
std::unique_ptr<E64::EngineCtx> E64::Engine::ctx = std::make_unique<E64::EngineCtx>();

void E64::Engine::run(){
    E64::IRenderer* renderer = ctx->renderer;

    float dt  = 0.0f;
    while(running){
        auto start = std::chrono::high_resolution_clock::now();

        if(ctx->input) { 
            ctx->input->poll(); 
            if(!ctx->input->isRunning()) exit();
        }
        else { E64::Log::error("No Input Attached"); E64::Engine::exit(); }

        if(renderer->isPendingResize())
        {
            renderer->ResizeViewport();
        }

        renderer->startFrame();
        for(Layer* layer : E64::Layer::layers){
            layer->OnUpdate(dt);
        }
        for (Layer* layer : E64::Layer::layers) {
            layer->OnRender();
        }
        for(Layer* layer : E64::Layer::layers){
            layer->OnImGuiRender();
        }
        renderer->submit();

        auto end = std::chrono::high_resolution_clock::now();
        if(ctx->mode != N64_RUNTIME) dt = std::chrono::duration<float>(end - start).count();
    }
}

void E64::Engine::exit(){
    running = false;
}
