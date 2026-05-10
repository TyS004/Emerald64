#ifndef Renderer_H
#define Renderer_H

#include "SDL3/SDL.h"
#include "ECS/Componet.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace E64{
    class Renderer{
        public:
            Renderer();
            ~Renderer();

            void OnResize(float width, float height);

            void beginSceneRenderPass();
            void beginUIRenderPass();
            void endRenderPass();

            void bindPipeline(SDL_GPUGraphicsPipeline* pipeline);
            void bindVertexBuffers(E64::ECS::Mesh* mesh);
            void bindIndexBuffers(E64::ECS::Mesh* mesh);

            void sendUniforms(glm::mat4 mvp);
            
            void draw(ECS::Mesh* mesh);
            void drawUI();
            void submit();

            SDL_GPURenderPass* getRenderPass();
            SDL_GPUTexture* getDepthTexture();
            SDL_GPUTexture* getSceneTexture();
        private:
            SDL_GPURenderPass* render_pass;
            SDL_GPUCommandBuffer* cmd_buf;
            SDL_GPUTexture* depth_texture;
            SDL_GPUTexture* scene_texture;
            SDL_GPUSampler* scene_sampler;

            SDL_GPUColorTargetInfo         color_target_info;
            SDL_GPUDepthStencilTargetInfo  depth_target_info;
            SDL_GPUTextureCreateInfo       depth_texture_info;
            SDL_GPUTextureCreateInfo       scene_texture_info;

            SDL_GPUTexture* swapchain;
            uint32_t width, height;
    };
}

#endif