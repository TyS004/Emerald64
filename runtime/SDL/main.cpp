#include <E64.h>
#include <Window/SDLWindow.h>
#include <Renderer/SDLRenderer.h>
#include <Input/SDLInput.h>

#include "Renderer/SDLRenderer.h"
#include "RuntimeLayer.h"

int main(){
    E64::Engine::ctx->mode = E64::E64_MODE::DESKTOP_RUNTIME;
    E64::Engine::ctx->root_dir = "../../../assets/";

    E64::SDLWindow* window = new E64::SDLWindow();
    window->create("E64 Runtime", 800, 600);
    E64::Engine::ctx->window = window;
    
    E64::SDLRenderer* renderer = new E64::SDLRenderer();
    E64::SDLInput* input = new E64::SDLInput();

    E64::Engine::ctx->renderer = renderer;
    E64::Engine::ctx->input = input;

    E64::SceneLayer scene_layer = E64::SceneLayer();
    E64::PhysicsLayer physics_layer = E64::PhysicsLayer();
    Runtime::RuntimeLayer runtime_layer = Runtime::RuntimeLayer();

    E64::Layer::Attach(&scene_layer);
    E64::Layer::Attach(&runtime_layer);
    
    E64::Engine::run();
    delete renderer;
}