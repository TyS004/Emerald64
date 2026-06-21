#include "Window/SDLWindow.h"
#include "Layer/Layer.h"

#define __APPLE__ 1

E64::SDLWindow::SDLWindow(){
    window = nullptr;
    device = nullptr;
    width = 1600;
    height = 900;
    isMouseLocked = false;
    VSync = false;
}

E64::SDLWindow::~SDLWindow(){
    SDL_DestroyWindow(window);
    SDL_DestroyGPUDevice(device);
    SDL_Quit();
}

void E64::SDLWindow::create(const char* name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) == 0){
        E64::Log::warn("SDL Could not Initalize");
        return;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if(!window) { E64::Log::warn("SDLWindow could not be created"); SDL_Quit();}

    E64::SDLWindow::width = width;
    E64::SDLWindow::height = height;

    #ifdef E64_APPLE
        device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    #else
        device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, nullptr);
    #endif

    if (!device) { E64::Log::error(SDL_GetError()); SDL_Quit(); }

    if (!SDL_ClaimWindowForGPUDevice(device, window)) {
        E64::Log::error("Couldnt Claim Window!");
        exit(1);
    }
    SDL_SetGPUSwapchainParameters(
        device,
        window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        SDL_GPU_PRESENTMODE_IMMEDIATE
    );
    SDL_SetHint(SDL_HINT_WINDOWS_RAW_KEYBOARD, "1");
}

SDL_Window* E64::SDLWindow::getWindow(){
    return window;
}

SDL_GPUDevice* E64::SDLWindow::getDevice(){
    return device;
}

uint32_t  E64::SDLWindow::getWidth(){
    return width;
}

uint32_t  E64::SDLWindow::getHeight(){
    return height;
}

bool E64::SDLWindow::getVSync(){
    return VSync;
}

void E64::SDLWindow::setMouseLock(bool lock){
    SDL_SetWindowRelativeMouseMode(window, lock);
    isMouseLocked = lock;
}

bool E64::SDLWindow::isMouseLock(){
    return isMouseLocked;
}

void E64::SDLWindow::toggleVSync(){
    VSync = !VSync;
    SDL_WaitForGPUIdle(device);
    SDL_SetGPUSwapchainParameters(device, window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        VSync ? SDL_GPU_PRESENTMODE_VSYNC : SDL_GPU_PRESENTMODE_IMMEDIATE);
}