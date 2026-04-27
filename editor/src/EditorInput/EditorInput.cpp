#include "EditorInput/EditorInput.h"
#include "Layer/EditorLayer.h"
#include <GCGameEngine.h>

using namespace GCGameEngine;

void Editor::EditorInput::OnKeyPressed(SDL_Scancode scancode){
    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_ESCAPE)) GCGameEngine::Engine::exit();
}

void Editor::EditorInput::OnKeyDown(SDL_Scancode scancode){
    
}