#include <GCGameEngine.h>
#include <SDL3/SDL.h>

#include "Layer/EditorLayer.h"

int main(){
    std::cout << "Starting Editor" << std::endl;
    GCGameEngine::Window::Create("Editor", 800, 600);

    Editor::EditorLayer* layer = new Editor::EditorLayer();
    GCGameEngine::Engine::pushLayer(layer);
    
    GCGameEngine::Engine::run();
    
    GCGameEngine::Window::Destory();
    delete layer;
}