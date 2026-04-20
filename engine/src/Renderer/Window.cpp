#include "Renderer/Window.h"
#include "Logger/Logger.h"

GCGameEngine::Log logger;

GCGameEngine::Window::Window(const char* name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        logger.warn("SDL Could not Initalize");
        return;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if(!window) { logger.warn("Window could not be created"); SDL_Quit();}

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
    SDL_ClaimWindowForGPUDevice(device, window);
}

GCGameEngine::Window::~Window(){
    SDL_DestroyWindow(window);
    SDL_DestroyGPUDevice(device);
    SDL_Quit();
}

bool GCGameEngine::Window::update(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_EVENT_QUIT){
            return false;
        }
    }
    return true;
}

SDL_Window* GCGameEngine::Window::getWindow(){
    return window;
}

SDL_GPUDevice* GCGameEngine::Window::getDevice(){
    return device;
}