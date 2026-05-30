#include <E64.h>

#include "EditorLayer/EditorLayer.h"
#include "SDLRenderer.h"

int main(){
    std::cout << "Starting Editor" << std::endl;
    E64::Window::Create("Emerald 64 Editor", 1300, 800);

    E64::Engine::ctx->Editor = true;

    SDLRenderer* renderer = new SDLRenderer();
    E64::Engine::ctx->renderer = renderer;

    Editor::EditorLayer editor_layer  = Editor::EditorLayer();
    E64::SceneLayer     scene_layer   = E64::SceneLayer();

    E64::Layer::Attach(&editor_layer);
    E64::Layer::Attach(&scene_layer);
    
    E64::Engine::run();

    E64::Window::Destory();
    delete renderer;
}