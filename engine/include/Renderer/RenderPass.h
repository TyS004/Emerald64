#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "SDL3/SDL.h"

namespace GCGameEngine{
    class RenderPass{
        public:
            static void begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* colorTargetInfo);
            static void end();
        private:
            static SDL_GPURenderPass* renderPass;
    };
}

#endif