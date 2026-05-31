#ifndef N64RENDERER_H
#define N64RENDERER_H

#include <E64.h>

class N64Renderer : public E64::IRenderer{
    public:
        N64Renderer();
        ~N64Renderer();
        
        void OnImGuiResize(float width, float height){};
        void ResizeViewport(){};

        void aquireCmdBufferandSwapChain(){};

        void bindPipeline(){};
        void bindVertexBuffers(E64::ECS::Mesh* mesh){};
        void bindIndexBuffers(E64::ECS::Mesh* mesh){};
        void bindFragmentSamplers(E64::ECS::Mesh* mesh){};

        void sendUniforms(glm::mat4 mvp){};

        void beginRenderPass(E64::RenderTarget target){};
        void endRenderPass(){};
        
        void draw(E64::ECS::Mesh* mesh){};
        void drawUI(){};
        void submit(){};

        E64::GPUBufferHandle createVertexBuffer(std::vector<E64::Vertex> vertices){ return 0; };
        E64::GPUBufferHandle createIndexBuffer(std::vector<E64::Index> indcies){ return 0; };
        E64::GPUTextureHandle createTexture(std::string path) { return 0; };
        E64::GPUSamplerHandle createSampler() { return 0; };

        bool isPendingResize(){return false;};
        
        int getDrawCalls(){return -1;};

};

#endif