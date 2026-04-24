#ifndef EVENT_H
#define EVENT_H

#include "SDL3/SDL.h"

#define GCENGINE_KEY_PRESSED 1

enum ENGINE_EVENT{
    KEYBOARD_PRESSED_EVENT,
    KEYBOARD_RELEASED_EVENT,
    WINDOW_EVENT,
    MOUSE_PRESSED_EVENT,
};

enum ENGINE_KEYBOARD_KEYCODE{
};
 
class Event{
    public:
        Event();

        virtual ENGINE_EVENT getType();
    private:
        SDL_Event event;
};

class KeyboardEvent : Event{
    public:
        KeyboardEvent();

        ENGINE_EVENT getType();
        uint8_t getKeycode();
    private:
        SDL_KeyboardEvent event;
};

#endif 