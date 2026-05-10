#include "Input/Input.h"
#include "Window/Window.h"

bool* E64::Input::pressed_keys = new bool[SDL_SCANCODE_COUNT];
bool E64::Input::running = true;

std::function<void(SDL_Scancode)>           E64::Input::OnKeyPressedBind   = E64::Input::OnKeyPressed;
std::function<void(SDL_Scancode)>           E64::Input::OnKeyDownBind      = E64::Input::OnKeyDown;
std::function<void(SDL_MouseMotionEvent)>   E64::Input::OnMouseMoveBind    = E64::Input::OnMouseMove;
std::function<void(SDL_WindowEvent)>        E64::Input::OnWindowResizeBind = E64::Input::OnWindowResize;

void E64::Input::processEvent(SDL_Event e){
    switch(e.type){
        case SDL_EVENT_KEY_DOWN:
            pressed_keys[e.key.scancode] = true;
            OnKeyPressedBind(e.key.scancode);
            OnKeyDownBind(e.key.scancode);
            break;
        case SDL_EVENT_KEY_UP:
            pressed_keys[e.key.scancode] = false;
            break;
        case SDL_EVENT_MOUSE_MOTION:
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
    }
}

bool E64::Input::isKeyPressed(SDL_Scancode scancode){
    return pressed_keys[scancode] ? true : false;
}

bool E64::Input::isKeyDown(SDL_Scancode scancode){
    return pressed_keys[scancode] ? true : false;
}

bool E64::Input::isRunning(){
    return running;
}

void E64::Input::clean(){
    delete[] pressed_keys;
}