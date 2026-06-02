#include <E64.h>
#include <Renderer/N64Renderer.h>
#include <Input/N64Input.h>

int main()
{
    E64::Engine::ctx->mode = E64::E64_MODE::N64_RUNTIME;
    
    E64::N64Renderer* renderer = new E64::N64Renderer();
    E64::N64Input* input = new E64::N64Input();
    E64::Engine::ctx->renderer = renderer;
    E64::Engine::ctx->input = input;

    E64::SceneLayer runtime_layer = E64::SceneLayer();
    E64::Layer::Attach(&runtime_layer);
    
    E64::Engine::run();
}