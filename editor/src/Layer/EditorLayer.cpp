#include "Layer/EditorLayer.h"

Editor::EditorLayer::EditorLayer(){
    scene = new GCGameEngine::Scene();
    GCGameEngine::Engine::setActiveScene(scene);
}

Editor::EditorLayer::~EditorLayer(){
    
}

void Editor::EditorLayer::OnUpdate(){
    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_A)){
        GCGameEngine::Log::debug("A Pressed!");
    }
}