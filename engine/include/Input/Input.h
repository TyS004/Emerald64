#ifndef INPUT_H
#define INPUT_H

#include "SDL3/SDL.h"

namespace E64{
    class Input{
        public:
            static bool isRunning();
            static void processEvent(SDL_Event e);

            static bool isKeyPressed(SDL_Scancode code);
            static bool isKeyDown(SDL_Scancode code);
            static void OnKeyPressed(SDL_Scancode scancode) {};
            static void OnKeyDown(SDL_Scancode scancode) {};

            static void OnMouseMove(SDL_MouseMotionEvent e) {};

            static void OnWindowResize(SDL_WindowEvent e) {};

            static void clean();

            static std::function<void(SDL_Scancode)> OnKeyPressedBind;
            static std::function<void(SDL_Scancode)> OnKeyDownBind;
            static std::function<void(SDL_MouseMotionEvent e)> OnMouseMoveBind;
            static std::function<void(SDL_WindowEvent e)> OnWindowResizeBind;
        private:
            static bool* pressed_keys;
            static bool running;
    };
}

#endif 