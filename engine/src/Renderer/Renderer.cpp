#include "Renderer/Renderer.h"

SDL_GPURenderPass* GCGameEngine::Renderer::render_pass = nullptr;

void GCGameEngine::Renderer::begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* color_target_info, SDL_GPUDepthStencilTargetInfo* depth_target_info){
    render_pass = SDL_BeginGPURenderPass(cmdBuffer, color_target_info, 1, depth_target_info);
}

void GCGameEngine::Renderer::end(){
    SDL_EndGPURenderPass(render_pass);
}

void GCGameEngine::Renderer::bindPipeline(SDL_GPUGraphicsPipeline* pipeline){
    SDL_BindGPUGraphicsPipeline(render_pass, pipeline);
}

void GCGameEngine::Renderer::bindVertexBuffers(GCGameEngine::ECS::Mesh* mesh){
    SDL_BindGPUVertexBuffers(render_pass, 0, mesh->vbo->getBufferBinding(), 1);
}

void GCGameEngine::Renderer::bindIndexBuffers(GCGameEngine::ECS::Mesh* mesh){
    SDL_GPUBuffer* index_buffer = mesh->ibo->getIndexBuffer();
    SDL_GPUBufferBinding binding;
    binding.buffer = index_buffer;
    binding.offset = 0;

    SDL_BindGPUIndexBuffer(render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
}

void GCGameEngine::Renderer::draw(GCGameEngine::ECS::Mesh* mesh){
    SDL_DrawGPUIndexedPrimitives(render_pass, mesh->ibo->getNumIndices(), 1, 0, 0, 0);
}

void GCGameEngine::Renderer::sendUniforms(SDL_GPUCommandBuffer* cmd_buf, glm::mat4 mvp){
    SDL_PushGPUVertexUniformData(cmd_buf, 0, &mvp, sizeof(mvp));
}