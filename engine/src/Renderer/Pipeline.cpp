#include "Renderer/Pipeline.h"
#include "Logger/Logger.h"

static GCGameEngine::Log logger = GCGameEngine::Log();

GCGameEngine::Pipeline::Pipeline(SDL_GPUDevice* device, const char* shaderPath){
    this->device = device;
    
    vertShader = new Shader(shaderPath, SDL_GPU_SHADERSTAGE_VERTEX, device);
    fragShader = new Shader(shaderPath, SDL_GPU_SHADERSTAGE_FRAGMENT, device);

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.vertex_shader = vertShader->getShader();
    pipelineInfo.fragment_shader = fragShader->getShader();

    pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

GCGameEngine::Pipeline::~Pipeline(){
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}