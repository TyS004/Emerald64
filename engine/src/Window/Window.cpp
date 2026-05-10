#include "Window/Window.h"
#include "Logger/Logger.h"
#include "Layer/Layer.h"

E64::Log logger;

SDL_Window* E64::Window::window = nullptr;
SDL_GPUDevice* E64::Window::device = nullptr;
uint32_t E64::Window::width = 1600;
uint32_t  E64::Window::height = 900;

bool E64::Window::isMouseLocked = false;

void E64::Window::Create(const char* name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) == 0){
        logger.warn("SDL Could not Initalize");
        return;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if(!window) { logger.warn("Window could not be created"); SDL_Quit();}

    E64::Window::width = width;
    E64::Window::height = height;

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_MSL, true, "metal");
    SDL_ClaimWindowForGPUDevice(device, window);
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
    if(window == nullptr) { E64::Log::error("NO WINDOW CREATED!"); }
    return window;
}

SDL_GPUDevice* E64::Window::getDevice(){
    if(device == nullptr) { E64::Log::error("NO DEVICE FOUND!"); }
    return device;
}

uint32_t  E64::Window::getWidth(){
    return width;
}

uint32_t  E64::Window::getHeight(){
    return height;
}

void E64::Window::setMouseLock(bool lock){
    SDL_SetWindowRelativeMouseMode(window, lock);
    isMouseLocked = lock;
}

bool E64::Window::isMouseLock(){
    return isMouseLocked;
}