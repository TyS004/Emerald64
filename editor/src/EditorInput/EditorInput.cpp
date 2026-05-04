#include "EditorInput/EditorInput.h"
#include "Layer/EditorLayer.h"
#include <GCGameEngine.h>

using namespace GCGameEngine;

int mouse_offset_x = 0;
int mouse_offset_y = 0;

void Editor::EditorInput::OnKeyPressed(SDL_Scancode scancode){
    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_ESCAPE)) GCGameEngine::Engine::exit();
}

void Editor::EditorInput::OnKeyDown(SDL_Scancode scancode){
    
}

void Editor::EditorInput::OnMouseMove(SDL_MouseMotionEvent e){
    //std::cout <<  "Mouse Offset: " << e.xrel << " " << e.yrel << std::endl;

    GCGameEngine::Camera* camera = Editor::EditorLayer::scene->getCamera();
    GCGameEngine::ECS::Transform* transform = camera->getTransform();

    if(GCGameEngine::Window::isMouseLock())
    {
        transform->euler.x += e.xrel * camera->getSens();
        transform->euler.y -= e.yrel * camera->getSens();
        transform->euler.y = glm::clamp(transform->euler.y, -89.0f, 89.0f);
    }
}