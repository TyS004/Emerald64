#include "Input/Input.h"
#include "Window/Window.h"
#include "Layer/PhysicsLayer.h"

bool E64::Input::running = true;

float E64::Input::xrel = 0.0f;
float E64::Input::yrel = 0.0f;

std::function<void(SDL_Scancode)>           E64::Input::OnKeyPressedBind   = E64::Input::OnKeyPressed;
std::function<void(SDL_Scancode)>           E64::Input::OnKeyDownBind      = E64::Input::OnKeyDown;
std::function<void(SDL_MouseMotionEvent)>   E64::Input::OnMouseMoveBind    = E64::Input::OnMouseMove;
std::function<void(SDL_WindowEvent)>        E64::Input::OnWindowResizeBind = E64::Input::OnWindowResize;
std::function<void(const char*)>            E64::Input::OnFileDroppedBind  = E64::Input::OnFileDropped;

void E64::Input::processEvent(SDL_Event e){
    switch(e.type){
        case SDL_EVENT_KEY_DOWN:
            OnKeyPressedBind(e.key.scancode);
            OnKeyDownBind(e.key.scancode);
            OnKeyDown(e.key.scancode);
            break;
        case SDL_EVENT_KEY_UP:
            break;
        case SDL_EVENT_MOUSE_MOTION:
            xrel = e.motion.xrel;
            yrel = e.motion.yrel;
            OnMouseMoveBind(e.motion);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            OnWindowResizeBind(e.window);
            break;
        case SDL_EVENT_QUIT:
            running = false;
            break;
        case SDL_EVENT_DROP_FILE:
            OnFileDroppedBind(e.drop.data);
            break;
        default:
            break;
    }
}

void E64::Input::OnKeyDown(SDL_Scancode scancode){
    if(scancode == SDL_SCANCODE_R) E64::PhysicsLayer::running = !E64::PhysicsLayer::running;
}

bool E64::Input::isKeyPressed(SDL_Scancode scancode){
    const bool* state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}

bool E64::Input::isKeyDown(SDL_Scancode scancode){
    const bool* state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}

bool E64::Input::isRunning(){
    return running;
}

float E64::Input::getXRel(){
    float x = xrel;
    xrel = 0.0f;
    return x;
}

float E64::Input::getYRel(){
    float y = yrel;
    yrel = 0.0f;
    return y;
}