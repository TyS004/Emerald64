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
            static float getXRel();
            static float getYRel();

            static void OnWindowResize(SDL_WindowEvent e) {};

            static void OnFileDropped(const char* data) {};

            static void clean();

            static std::function<void(SDL_Scancode)>           OnKeyPressedBind;
            static std::function<void(SDL_Scancode)>           OnKeyDownBind;
            static std::function<void(SDL_MouseMotionEvent)>   OnMouseMoveBind;
            static std::function<void(SDL_WindowEvent)>        OnWindowResizeBind;
            static std::function<void(const char*)>            OnFileDroppedBind;
        private:
            static bool* pressed_keys;
            static float xrel;
            static float yrel;
            static bool running;
    };
}

#endif 