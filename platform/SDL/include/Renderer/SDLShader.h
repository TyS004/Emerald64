#ifndef SDLSHADER_H
#define SDLSHADER_H

#include <SDL3/SDL.h>

namespace E64 {
    class SDLShader{
        public:
            SDLShader(const char* path, SDL_GPUShaderStage stage, SDL_GPUDevice* device);
            ~SDLShader();

            SDL_GPUShader* getShader();
        private:
            SDL_GPUShader* loadShader();

            const char* path;
            SDL_GPUShaderStage stage;
            SDL_GPUShader* shader;
            SDL_GPUDevice* device;
    };
}

#endif