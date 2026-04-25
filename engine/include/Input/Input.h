#ifndef INPUT_H
#define INPUT_H

#include "SDL3/SDL.h"

namespace GCGameEngine{
    class Input{
        public:
            static void processEvent(SDL_Event e);

            static bool isKeyPressed(SDL_Scancode code);
            static bool isKeyDown(SDL_Scancode code);

            static bool isRunning();
        private:
            static bool* pressed_keys;
            static bool running;
    };
}

#endif 