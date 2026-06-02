#ifndef SDLPIPELINE_H
#define SDLPIPELINE_H

#include <SDL3/SDL.h>
#include <E64.h>

#include "SDLShader.h"

namespace E64 {
    class SDLPipeline{
        public:
            SDLPipeline() {}
            SDLPipeline(const char* shader_path);

            ~SDLPipeline();

            SDL_GPUGraphicsPipeline* getPipeline();
        private:
            SDL_GPUDevice* device;
            SDLShader* vert_shader;
            SDLShader* frag_shader;

            SDL_GPUGraphicsPipeline* pipeline;
    };
}

#endif