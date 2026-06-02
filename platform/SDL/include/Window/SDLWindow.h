#ifndef WINDOW
#define WINDOW

#include <SDL3/SDL.h>
#include <E64.h>

namespace E64{
    class SDLWindow : public Window
    {
        public:
            SDLWindow();
            ~SDLWindow();

            void create(const char* name, int width, int height);
            void destory();
        
            SDL_Window* getWindow();
            SDL_GPUDevice* getDevice();

            void setMouseLock(bool lock);
            bool isMouseLock();

            uint32_t getWidth();
            uint32_t  getHeight();
            bool getVSync();

            void toggleVSync();

        private:
            SDL_Window* window;
            SDL_GPUDevice* device;

            uint32_t width;
            uint32_t height;

            bool isMouseLocked;
            bool VSync;
    };
}

#endif