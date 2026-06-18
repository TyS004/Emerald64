#ifndef SDLPIPELINE_H
#define SDLPIPELINE_H

#include <SDL3/SDL.h>
#include <E64.h>

#include "SDLShader.h"

namespace E64 {
    class SDLPipeline{
        public:
            SDLPipeline() {}
            SDLPipeline(const char* shader_path, SDL_GPUFillMode mode = SDL_GPU_FILLMODE_FILL);

            ~SDLPipeline();

            SDL_GPUGraphicsPipeline* getPipeline();
            void rebuildPipeline();

            SDL_GPUFillMode getFillMode();
        private:
            SDL_GPUDevice* device;
            SDLShader* vert_shader;
            SDLShader* frag_shader;

            SDL_GPUFillMode fill_mode;

            SDL_GPUGraphicsPipelineCreateInfo pipelineInfo;
            SDL_GPUGraphicsPipeline* pipeline;
    };
}

#endif