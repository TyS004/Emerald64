#ifndef INPUT_H
#define INPUT_H

#include "SDL3/SDL.h"

namespace GCGameEngine{
    class Input{
        public:
            static bool isRunning();
            static void processEvent(SDL_Event e);

            static bool isKeyPressed(SDL_Scancode code);
            static bool isKeyDown(SDL_Scancode code);
            static void OnKeyPressed(SDL_Scancode scancode) {};
            static void OnKeyDown(SDL_Scancode scancode) {};

            static void clean();

            static std::function<void(SDL_Scancode)> OnKeyPressedBind;
            static std::function<void(SDL_Scancode)> OnKeyDownBind;
        private:
            static bool* pressed_keys;
            static bool running;
    };
}

#endif 