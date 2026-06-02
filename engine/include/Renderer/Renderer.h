#ifndef Renderer_H
#define Renderer_H

#include "RendererHandle.h"

#include "ECS/Component.h"

namespace E64{
    enum RenderTarget{
        SWAPCHAIN,
        TEXTURE
    };

    class IRenderer{
        public:
            virtual void OnImGuiResize(float width, float height){};
            virtual void ResizeViewport(){};

            virtual void aquireCmdBufferandSwapChain(){};

            virtual void bindPipeline(){};
            virtual void bindVertexBuffers(E64::ECS::Mesh* mesh){};
            virtual void bindIndexBuffers(E64::ECS::Mesh* mesh){};
            virtual void bindFragmentSamplers(E64::ECS::Mesh* mesh){};

            virtual void sendUniforms(glm::mat4 mvp){};

            virtual void beginRenderPass(RenderTarget target){};
            virtual void endRenderPass(){};
            
            virtual void draw(ECS::Mesh* mesh){};
            virtual void drawUI(){};
            virtual void submit(){};

            virtual GPUBufferHandle createVertexBuffer(std::vector<Vertex> vertices){ return 0; };
            virtual GPUBufferHandle createIndexBuffer(std::vector<Index> indcies){ return 0; };
            virtual GPUTextureHandle createTexture(std::string path) { return 0; };
            virtual GPUSamplerHandle createSampler() { return 0; };

            virtual bool isPendingResize(){return false;};
            
            virtual int getDrawCalls(){return -1;};
    };
}

#endif