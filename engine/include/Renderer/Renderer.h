#ifndef Renderer_H
#define Renderer_H

#include "SDL3/SDL.h"

namespace GCGameEngine{
    class Renderer{
        public:
            static void begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* colorTargetInfo);
            static void end();

            static void bindPipeline(SDL_GPUGraphicsPipeline* pipeline);
            static void bindVertexBuffers(SDL_GPUBufferBinding* bindings);
            static void draw();
        private:
            static SDL_GPURenderPass* renderPass;
    };
}

#endif