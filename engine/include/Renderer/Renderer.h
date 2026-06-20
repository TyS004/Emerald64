#ifndef Renderer_H
#define Renderer_H

#include "RendererHandle.h"

#include "ECS/Component.h"
#include "AssetManager/Assets.h"

namespace E64{
    enum RenderTarget{
        SWAPCHAIN,
        TEXTURE
    };

    enum RenderLoadOP{
        CLEAR,
        LOAD
    };

    enum RenderStoreOP{
        STORE,
        RESOLVE
    };

    class IRenderer{
        public:
            virtual void OnImGuiResize(float width, float height){};
            virtual void ResizeViewport(){};

            virtual void startFrame(){};

            virtual void bindPipeline(){};
            virtual void bindVertexBuffers(ECS::MeshComponent* comp){};
            virtual void bindIndexBuffers(ECS::MeshComponent* comp){};
            virtual void bindFragmentSamplers(ECS::MeshComponent* comp){};

            virtual void pushVertexUniform(const void* data, size_t size, uint32_t slot) = 0;
            virtual void pushFragmentUniform(const void* data, size_t size, uint32_t slot) = 0;

            virtual void beginRenderPass(RenderTarget target){};
            virtual void endRenderPass(){};
            
            virtual void setColorLoadOP(RenderLoadOP OP) {};
            virtual void setColorStoreOP(RenderStoreOP OP) {};
            virtual void setDepthLoadOP(RenderLoadOP OP) {};
            virtual void setDepthStoreOP(RenderStoreOP OP) {};

            virtual void setStencilReference(int ref) {};

            virtual void draw(ECS::MeshComponent* comp) {};
            virtual void drawUI(){};
            virtual void submit(){};

            virtual GPUBufferHandle createVertexBuffer(std::vector<Vertex> vertices){ return 0; };
            virtual GPUBufferHandle createIndexBuffer(std::vector<Index> indcies){ return 0; };
            virtual GPUTextureHandle createTexture(unsigned char* img_data, int width, int height) { return 0; };
            virtual GPUSamplerHandle createSampler() { return 0; };

            virtual bool isPendingResize(){return false;};
            
            virtual int getDrawCalls(){return -1;};
    };
}

#endif