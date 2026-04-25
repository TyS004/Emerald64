#include "Input/Input.h"

bool* GCGameEngine::Input::pressed_keys = new bool[SDL_SCANCODE_COUNT];
bool GCGameEngine::Input::running = true;

void GCGameEngine::Input::processEvent(SDL_Event e){
    switch(e.type){
        case SDL_EVENT_KEY_DOWN:
            pressed_keys[e.key.scancode] = true;
            break;
        case SDL_EVENT_KEY_UP:
            pressed_keys[e.key.scancode] = false;
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