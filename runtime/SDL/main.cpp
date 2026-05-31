#include <E64.h>

#include "SDLRenderer.h"

int main(){
    E64::Window::Create("E64 Runtime", 800, 600);
    SDLRenderer* renderer = new SDLRenderer();
    E64::Engine::ctx->renderer = renderer;

    E64::SceneLayer runtime_layer = E64::SceneLayer();
    E64::Layer::Attach(&runtime_layer);
    
    E64::Engine::run();
    delete renderer;
}