#include "Window/Window.h"
#include "Layer/Layer.h"

#define __APPLE__ 1

SDL_Window* E64::Window::window = nullptr;
SDL_GPUDevice* E64::Window::device = nullptr;
uint32_t E64::Window::width = 1600;
uint32_t  E64::Window::height = 900;

bool E64::Window::isMouseLocked = false;
bool E64::Window::VSync = false;

void E64::Window::Create(const char* name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) == 0){
        E64::Log::warn("SDL Could not Initalize");
        return;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if(!window) { E64::Log::warn("Window could not be created"); SDL_Quit();}

    E64::Window::width = width;
    E64::Window::height = height;

    if(__APPLE__) device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    else          device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
    if(!device) { E64::Log::error(SDL_GetError()); SDL_Quit(); }

    SDL_ClaimWindowForGPUDevice(device, window);
    SDL_SetGPUSwapchainParameters(
        device,
        window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        SDL_GPU_PRESENTMODE_IMMEDIATE
    );
    SDL_SetHint(SDL_HINT_WINDOWS_RAW_KEYBOARD, "1");
}

void E64::Window::Destory(){
    SDL_DestroyWindow(window);
    SDL_DestroyGPUDevice(device);
    SDL_Quit();
}

SDL_Event E64::Window::PollEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        E64::Input::processEvent(event);
        for(Layer* layer : Layer::layers){
            layer->OnEvent(&event);
        }
    }
    return event;
}

SDL_Window* E64::Window::getWindow(){
    return window;
}

SDL_GPUDevice* E64::Window::getDevice(){
    return device;
}

uint32_t  E64::Window::getWidth(){
    return width;
}

uint32_t  E64::Window::getHeight(){
    return height;
}

bool E64::Window::getVSync(){
    return VSync;
}

void E64::Window::setMouseLock(bool lock){
    SDL_SetWindowRelativeMouseMode(window, lock);
    isMouseLocked = lock;
}

bool E64::Window::isMouseLock(){
    return isMouseLocked;
}

void E64::Window::toggleVSync(){
    VSync = !VSync;
    SDL_WaitForGPUIdle(device);
    SDL_SetGPUSwapchainParameters(device, window,
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
        VSync ? SDL_GPU_PRESENTMODE_VSYNC : SDL_GPU_PRESENTMODE_IMMEDIATE);
}