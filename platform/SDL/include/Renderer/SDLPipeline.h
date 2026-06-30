#ifndef SDLPIPELINE_H
#define SDLPIPELINE_H

#include <SDL3/SDL.h>
#include <E64.h>

#include "SDLShader.h"

namespace E64 {
    class SDLPipeline{
        public:
            SDLPipeline() {}
            SDLPipeline(const char* shaderPath, int vert_uniforms, int frag_uniforms, SDL_GPUFillMode mode = SDL_GPU_FILLMODE_FILL);
            SDLPipeline(const char* vertexShaderPath, const char* fragementShaderPath, int vert_uniforms, int frag_uniforms, SDL_GPUFillMode mode = SDL_GPU_FILLMODE_FILL);

            ~SDLPipeline();

            SDL_GPUGraphicsPipeline* getPipeline();
            void buildPipeline();

            SDL_GPUFillMode getFillMode();

            SDL_GPUDepthStencilState* getDepthStencilState();
            SDL_GPUGraphicsPipelineCreateInfo* getPipelineInfo();

        private:
            SDL_GPUDevice* device;

            SDLShader* vert_shader;
            SDLShader* frag_shader;

            SDL_GPUFillMode fill_mode;

            SDL_GPUDepthStencilState stencil_state;
            SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
            SDL_GPUVertexAttribute vertexAttributes[4];
            SDL_GPUColorTargetDescription colorTargetDesc;
            SDL_GPUTextureFormat fmt;
            SDL_GPUGraphicsPipelineCreateInfo pipelineInfo;
            SDL_GPUGraphicsPipeline* pipeline;
    };
}

#endif