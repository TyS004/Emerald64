#include "Renderer/Renderer.h"

SDL_GPURenderPass* GCGameEngine::Renderer::renderPass = nullptr;

void GCGameEngine::Renderer::begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* colorTargetInfo){
    renderPass = SDL_BeginGPURenderPass(cmdBuffer, colorTargetInfo, 1, NULL);
}

void GCGameEngine::Renderer::end(){
    SDL_EndGPURenderPass(renderPass);
}

void GCGameEngine::Renderer::bindPipeline(SDL_GPUGraphicsPipeline* pipeline){
    SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
}

void GCGameEngine::Renderer::bindVertexBuffers(SDL_GPUBufferBinding* binding){
    SDL_BindGPUVertexBuffers(renderPass, 0, binding, 1);
}

void GCGameEngine::Renderer::draw(){
    SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
}