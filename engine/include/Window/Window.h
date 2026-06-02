#ifndef WINDOW
#define WINDOW

#include <SDL3/SDL.h>
#include "Input/Input.h"

namespace E64{
    class Window
    {
        public:
            static void Create(const char* name, int width, int height);
            static void Destory();
        
            static SDL_Window* getWindow();
            static SDL_GPUDevice* getDevice();

            static void setMouseLock(bool lock);
            static bool isMouseLock();

            static uint32_t getWidth();
            static uint32_t  getHeight();
            static bool getVSync();

            static void toggleVSync();

        private:
            static SDL_Window* window;
            static SDL_GPUDevice* device;

            static uint32_t width;
            static uint32_t height;

            static bool isMouseLocked;
            static bool VSync;
    };
}

#endif