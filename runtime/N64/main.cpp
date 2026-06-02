#include <E64.h>

#define E64_N64_RUNTIME

#include <libdragon.h>
#include <t3d/t3d.h>

#include "N64Renderer.h"

int main()
{
    E64::Engine::ctx->mode = E64::E64_MODE::N64_RUNTIME;
    
    N64Renderer* renderer = new N64Renderer();
    E64::N64Input* input = new E64::N64Input();
    E64::Engine::ctx->renderer = renderer;
    E64::Engine::ctx->input = input;

    E64::SceneLayer runtime_layer = E64::SceneLayer();
    E64::Layer::Attach(&runtime_layer);
    
    E64::Engine::run();
}