#include <E64.h>
#include <Renderer/N64Renderer.h>
#include <Input/N64Input.h>
#include <Layer/N64Layer.h>

int main()
{
    E64::Engine::ctx->mode = E64::E64_MODE::N64_RUNTIME;
    E64::Engine::ctx->root_dir = "rom:/";
    
    E64::N64Window* window = new E64::N64Window(E64::RDP, 3);
    E64::Engine::ctx->window = window;

    E64::N64Renderer* renderer = new E64::N64Renderer();
    E64::Engine::ctx->renderer = renderer;

    E64::N64Input* input = new E64::N64Input();
    E64::Engine::ctx->input = input;

    E64::N64Layer n64_layer = E64::N64Layer();
    E64::Layer::Attach(&n64_layer);
    
    E64::Engine::run();
}