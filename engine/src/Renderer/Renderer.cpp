#include "Renderer/Renderer.h"

SDL_GPURenderPass* GCGameEngine::Renderer::render_pass = nullptr;

void GCGameEngine::Renderer::begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* colorTargetInfo){
    render_pass = SDL_BeginGPURenderPass(cmdBuffer, colorTargetInfo, 1, NULL);
}

void GCGameEngine::Renderer::end(){
    SDL_EndGPURenderPass(render_pass);
}

void GCGameEngine::Renderer::bindPipeline(SDL_GPUGraphicsPipeline* pipeline){
    SDL_BindGPUGraphicsPipeline(render_pass, pipeline);
}

void GCGameEngine::Renderer::bindVertexBuffers(SDL_GPUBufferBinding* binding){
    SDL_BindGPUVertexBuffers(render_pass, 0, binding, 1);
}

void GCGameEngine::Renderer::draw(){
    SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
}

void GCGameEngine::Renderer::sendUniforms(SDL_GPUCommandBuffer* cmd_buf, glm::mat4 mvp){
    SDL_PushGPUVertexUniformData(cmd_buf, 0, &mvp, sizeof(mvp));
}