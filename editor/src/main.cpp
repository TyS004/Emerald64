#include <E64.h>
#include <Window/SDLWindow.h>
#include <Renderer/SDLRenderer.h>

#include "EditorLayer/EditorLayer.h"
#include "EditorInput/EditorInput.h"

int main(){
    E64::Engine::ctx->mode = E64::E64_MODE::EDITOR;
    E64::Engine::ctx->root_dir = "../assets/";
    
    std::cout << "Starting Editor" << std::endl;

    E64::SDLWindow* window = new E64::SDLWindow();
    window->create("Emerald 64 Editor", 1300, 800);
    E64::Engine::ctx->window = window;

    E64::SDLRenderer* renderer = new E64::SDLRenderer();
    E64::Engine::ctx->renderer = renderer;
    
    Editor::EditorInput* input = new Editor::EditorInput();
    E64::Engine::ctx->input    = input;

    Editor::EditorLayer editor_layer  = Editor::EditorLayer();
    E64::SceneLayer     scene_layer   = E64::SceneLayer();

    E64::Layer::Attach(&editor_layer);
    E64::Layer::Attach(&scene_layer);
    
    E64::Engine::run();

    delete renderer;
    delete input;
    delete window;
}