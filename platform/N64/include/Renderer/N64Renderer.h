#ifndef N64RENDERER_H
#define N64RENDERER_H

#include <E64.h>
#include "Camera/N64Camera.h"
#include "Window/N64Window.h"

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

namespace E64 {
    class N64Renderer : public E64::IRenderer{
        public:
            N64Renderer();
            ~N64Renderer();
            
            void OnImGuiResize(float width, float height){};
            void ResizeViewport(){};

            void startFrame();

            void bindPipeline(){};
            void bindVertexBuffers(E64::Mesh* mesh){};
            void bindIndexBuffers(E64::Mesh* mesh){};
            void bindFragmentSamplers(E64::Mesh* mesh){};

            void sendUniforms(glm::mat4 mvp){};
            void sendModelMat4FP(T3DMat4FP* modelMatFP);

            void beginRenderPass(E64::RenderTarget target){};
            void endRenderPass(){};
            
            void draw(E64::Mesh* mesh);
            void drawText();
            void drawUI(){};
            void submit();

            E64::GPUBufferHandle createVertexBuffer(std::vector<E64::Vertex> vertices){ return 0; };
            E64::GPUBufferHandle createIndexBuffer(std::vector<E64::Index> indcies){ return 0; };
            E64::GPUTextureHandle createTexture(std::string path) { return 0; };
            E64::GPUSamplerHandle createSampler() { return 0; };

            bool isPendingResize(){return false;};
            
            int getDrawCalls(){return -1;};
            T3DViewport* getViewport();

            std::vector<std::string> debug_msg_queue;
        private:
            glm::mat4 mvp;
            T3DViewport viewport;

            N64Window* n64_window;

            T3DVertPacked* vertices;
            rspq_block_t* dplDraw;

            fm_mat4_t modelMat;
            T3DMat4FP* modelMatFP;

            std::vector<uint8_t> colorAmbient;
            std::vector<uint8_t> colorDir;
            fm_vec3_t lightDirVec;

            surface_t* frame_buf;
            surface_t* z_buf;
    };
}

#endif