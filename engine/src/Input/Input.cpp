#include "Input/Input.h"
#include "Window/Window.h"

bool* GCGameEngine::Input::pressed_keys = new bool[SDL_SCANCODE_COUNT];
bool GCGameEngine::Input::running = true;

std::function<void(SDL_Scancode)> GCGameEngine::Input::OnKeyPressedBind = GCGameEngine::Input::OnKeyPressed;
std::function<void(SDL_Scancode)> GCGameEngine::Input::OnKeyDownBind = GCGameEngine::Input::OnKeyDown;
std::function<void(SDL_MouseMotionEvent e)> GCGameEngine::Input::OnMouseMoveBind = GCGameEngine::Input::OnMouseMove;

void GCGameEngine::Input::processEvent(SDL_Event e){
    switch(e.type){
        case SDL_EVENT_KEY_DOWN:
            pressed_keys[e.key.scancode] = true;
            OnKeyPressedBind(e.key.scancode);
            break;
        case SDL_EVENT_KEY_UP:
            pressed_keys[e.key.scancode] = false;
            break;
        case SDL_EVENT_MOUSE_MOTION:
            OnMouseMoveBind(e.motion);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            SDL_HideCursor();
            GCGameEngine::Window::setMouseLock();
            break;
        case SDL_EVENT_QUIT:
            running = false;
            break;
    }
}

bool GCGameEngine::Input::isKeyPressed(SDL_Scancode scancode){
    return pressed_keys[scancode] ? true : false;
}

bool GCGameEngine::Input::isKeyDown(SDL_Scancode scancode){
    return pressed_keys[scancode] ? true : false;
}

bool GCGameEngine::Input::isRunning(){
    return running;
}

void GCGameEngine::Input::clean(){
    delete[] pressed_keys;
}