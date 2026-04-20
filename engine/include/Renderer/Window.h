#ifndef WINDOW
#define WINDOW

#include <SDL3/SDL.h>

namespace GCGameEngine{
    class Window
    {
        public:
            Window(const char* name, int width, int height);
            ~Window();

            bool update();

            SDL_Window* getWindow();
            SDL_GPUDevice* getDevice();
            SDL_GPUTextureFormat getSwapchainFormat();
        private:
            SDL_Window* window;
            SDL_GPUDevice* device;
            SDL_GPUTextureFormat swapchainFormat;
    };
}

#endif