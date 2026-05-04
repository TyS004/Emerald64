#ifndef Renderer_H
#define Renderer_H

#include "SDL3/SDL.h"
#include "ECS/Componet.h"

namespace GCGameEngine{
    class Renderer{
        public:
            static void begin(SDL_GPUCommandBuffer* cmd_buffer, SDL_GPUColorTargetInfo* color_target_info, SDL_GPUDepthStencilTargetInfo* depth_target_info);
            static void end();

            static void bindPipeline(SDL_GPUGraphicsPipeline* pipeline);
            static void bindVertexBuffers(GCGameEngine::ECS::Mesh* mesh);
            static void bindIndexBuffers(GCGameEngine::ECS::Mesh* mesh);

            static void sendUniforms(SDL_GPUCommandBuffer* cmd_buf, glm::mat4 mvp);
            
            static void draw(ECS::Mesh* mesh);
        private:
            static SDL_GPURenderPass* render_pass;
    };
}

#endif