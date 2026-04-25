#ifndef WINDOW
#define WINDOW

#include <SDL3/SDL.h>
#include "Input/Input.h"

namespace GCGameEngine{
    class Window
    {
        public:
            static void Create(const char* name, int width, int height);
            static void Destory();

            static SDL_Event PollEvent();

            static SDL_Window* getWindow();
            static SDL_GPUDevice* getDevice();
        private:
            static SDL_Window* window;
            static SDL_GPUDevice* device;
    };
}

#endif