#include "Renderer/RenderPass.h"

SDL_GPURenderPass* GCGameEngine::RenderPass::renderPass = nullptr;

void GCGameEngine::RenderPass::begin(SDL_GPUCommandBuffer* cmdBuffer, SDL_GPUColorTargetInfo* colorTargetInfo){
    renderPass = SDL_BeginGPURenderPass(cmdBuffer, colorTargetInfo, 1, NULL);
}

void GCGameEngine::RenderPass::end(){
    SDL_EndGPURenderPass(renderPass);
}