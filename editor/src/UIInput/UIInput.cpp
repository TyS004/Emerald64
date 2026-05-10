#include "UIInput/UIInput.h"
#include <E64.h>

using namespace E64;

int mouse_offset_x = 0;
int mouse_offset_y = 0;

Editor::EditorCamera* Editor::UIInput::camera = new Editor::EditorCamera();

void Editor::UIInput::OnKeyPressed(SDL_Scancode scancode){
    if(E64::Input::isKeyPressed(SDL_SCANCODE_ESCAPE)) E64::Engine::exit();
}

void Editor::UIInput::OnKeyDown(SDL_Scancode scancode){
    switch(scancode){
        case SDL_SCANCODE_Z:
            if(E64::Window::isMouseLock())
            {
                E64::Window::setMouseLock(false);
            }
            else{
                SDL_HideCursor();
                E64::Window::setMouseLock(true);
            }
            break;
        default:
            break;
    }
}

void Editor::UIInput::OnMouseMove(SDL_MouseMotionEvent e){
    E64::ECS::Transform* transform = camera->getTransform();

    if(E64::Window::isMouseLock())
    {
        transform->euler.x += e.xrel * camera->getSens();
        transform->euler.y -= e.yrel * camera->getSens();
        transform->euler.y = glm::clamp(transform->euler.y, -89.0f, 89.0f);
    }
    // std::string msg =  "UI Layer Mouse Move Event: " + std::to_string(e.x) + ", " + std::to_string(e.y);
    // E64::Log::debug(msg);
}

void Editor::UIInput::OnWindowResize(float width, float height){
    E64::Engine::ctx->renderer->OnResize(width, height);

    std::string msg =  "UI Layer Window Resize Event: " + std::to_string(width) + ", " + std::to_string(height);
    E64::Log::debug(msg);
}

Editor::EditorCamera* Editor::UIInput::getCamera(){
    return camera;
}