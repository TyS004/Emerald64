#include "Window/Window.h"
#include "Logger/Logger.h"

GCGameEngine::Log logger;

SDL_Window* GCGameEngine::Window::window = nullptr;
SDL_GPUDevice* GCGameEngine::Window::device = nullptr;

void GCGameEngine::Window::Create(const char* name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        logger.warn("SDL Could not Initalize");
        return;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if(!window) { logger.warn("Window could not be created"); SDL_Quit();}

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, "metal");
    SDL_ClaimWindowForGPUDevice(device, window);
}

void GCGameEngine::Window::Destory(){
    SDL_DestroyWindow(window);
    SDL_DestroyGPUDevice(device);
    SDL_Quit();
}

SDL_Event GCGameEngine::Window::PollEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        GCGameEngine::Input::processEvent(event);
    }
    return event;
}

SDL_Window* GCGameEngine::Window::getWindow(){
    return window;
}

SDL_GPUDevice* GCGameEngine::Window::getDevice(){
    return device;
}