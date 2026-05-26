#ifndef Renderer_H
#define Renderer_H

#include "SDL3/SDL.h"
#include "ECS/Component.h"
#include "Pipeline.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace E64{
    enum RenderTarget{
        SWAPCHAIN,
        TEXTURE
    };

    class Renderer{
        public:
            Renderer();
            ~Renderer();

            void OnImGuiResize(float width, float height);
            void ResizeViewport();
            void OnResize(float width, float height);

            void aquireCmdBufferandSwapChain();

            void beginRenderPass(RenderTarget target);
            void endRenderPass();

            void bindPipeline();
            void bindVertexBuffers(E64::ECS::Mesh* mesh);
            void bindIndexBuffers(E64::ECS::Mesh* mesh);
            void bindFragmentSamplers(E64::ECS::Mesh* mesh);

            void sendUniforms(glm::mat4 mvp);
            
            void draw(ECS::Mesh* mesh);
            void drawUI();
            void submit();

            bool isPendingResize();

            SDL_GPURenderPass* getRenderPass();
            SDL_GPUCommandBuffer* getCommandBuffer();
            SDL_GPUTexture* getDepthTexture();
            SDL_GPUTexture* getSceneTexture();
            int getDrawCalls();
        private:
            std::unique_ptr<E64::Pipeline> pipeline;

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

            int draw_calls;
            bool pending_resize;
    };
}

#endif