#include <GCGameEngine.h>
#include <SDL3/SDL.h>

#include "Layer/EditorLayer.h"

int main(){
    std::cout << "Starting Editor" << std::endl;
    
    GCGameEngine::Engine::pushLayer(new Editor::EditorLayer());
    GCGameEngine::Engine::run();
}