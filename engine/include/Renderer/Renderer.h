#ifndef Renderer_H
#define Renderer_H

#include "SDL3/SDL.h"
#include "Object/Object.h"

namespace GCGameEngine{
    class Renderer{
        public:
            static void begin(SDL_GPUCommandBuffer* cmd_buffer, SDL_GPUColorTargetInfo* color_target_info);
            static void end();

            static void bindPipeline(SDL_GPUGraphicsPipeline* pipeline);
            static void sendUniforms(SDL_GPUCommandBuffer* cmd_buf, Object* obj);
            static void bindVertexBuffers(SDL_GPUBufferBinding* bindings);
            static void draw();
        private:
            static SDL_GPURenderPass* render_pass;
    };
}

#endif