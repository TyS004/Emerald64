#ifndef SDLRenderer_H
#define SDLRenderer_H

#include <E64.h>

#include "SDLPipeline.h"

namespace E64 {
    class SDLRenderer : public IRenderer{
        public:
            SDLRenderer();
            ~SDLRenderer();

            void OnImGuiResize(float width, float height);
            void ResizeViewport();

            void startFrame();

            void bindPipeline();
            void bindVertexBuffers(E64::ECS::MeshComponent* comp);
            void bindIndexBuffers(E64::ECS::MeshComponent* comp);
            void bindFragmentSamplers(E64::ECS::MeshComponent* comp);

            void pushVertexUniform(const void* data, size_t size, uint32_t slot);
            void pushFragmentUniform(const void* data, size_t size, uint32_t slot);

            void beginRenderPass(E64::RenderTarget target);
            void endRenderPass();
            
            void draw(E64::ECS::MeshComponent* comp);
            void drawUI();
            void submit();

            E64::GPUBufferHandle createVertexBuffer(std::vector<E64::Vertex> vertices);
            E64::GPUBufferHandle createIndexBuffer(std::vector<E64::Index> indices);
            E64::GPUTextureHandle createTexture(unsigned char* img_data, int  width, int height);
            E64::GPUTextureHandle createSampler();

            bool isPendingResize();

            SDL_GPUCommandBuffer* getCommandBuffer();
            SDL_GPUTexture* getSceneTexture();

            SDLPipeline* getPipeline();
            void setPipeline(const char* shaderpath, SDL_GPUFillMode fill_mode);

            int getDrawCalls();

            void setColorTargetLoadOP(SDL_GPULoadOp load_op);
            void setDepthTargetLoadOP(SDL_GPULoadOp load_op);

        private:
            SDL_Window* window;
            SDL_GPUDevice* device;

            RenderTarget target;

            std::unique_ptr<SDLPipeline> pipeline;

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