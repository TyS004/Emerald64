#ifndef SHADER_H
#define SHADER_H

#include "SDL3/SDL.h"

namespace E64{
    class Shader{
        public:
            Shader(const char* path, SDL_GPUShaderStage stage, SDL_GPUDevice* device);
            ~Shader();

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