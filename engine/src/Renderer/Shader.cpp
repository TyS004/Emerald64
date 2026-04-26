#include "Renderer/Shader.h"
#include "Logger/Logger.h"

static GCGameEngine::Log logger = GCGameEngine::Log();

GCGameEngine::Shader::Shader(const char* path, SDL_GPUShaderStage stage, SDL_GPUDevice* device){
    this->device = device;
    this->stage = stage;
    this->path = path;
    this->shader = loadShader();
}

GCGameEngine::Shader::~Shader(){
    SDL_ReleaseGPUShader(this->device, this->shader);
}

SDL_GPUShader* GCGameEngine::Shader::loadShader(){
    char fullPath[256];
    if(stage == SDL_GPU_SHADERSTAGE_VERTEX){
        snprintf(fullPath, sizeof(fullPath), "%s_vert.metal", path);
    }
    else{
        snprintf(fullPath, sizeof(fullPath), "%s_frag.metal", path);
    }

    size_t codeSize;
    void* code = SDL_LoadFile(fullPath, &codeSize);
    if(!code){  
        logger.warn("Failed to Load Shader File");
        logger.warn(fullPath);
        return nullptr;
    }

    SDL_GPUShaderCreateInfo info = {};
    info.code = (const Uint8*) code;
    info.code_size = codeSize;
    info.format = SDL_GPU_SHADERFORMAT_MSL;
    info.stage = stage;
    info.num_samplers = 0;
    info.num_storage_buffers = 0;
    info.num_storage_textures = 0;

    if(stage == SDL_GPU_SHADERSTAGE_VERTEX){
        info.num_uniform_buffers = 1;
        info.entrypoint = "vertex_main";
    }
    else{
        info.num_uniform_buffers = 0;
        info.entrypoint = "fragment_main";
    }

    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
    SDL_free(code);

    if(!shader){
        logger.warn("Failed to Create Shader");
        return nullptr;
    }
    std::string msg = "Successfully Loaded Shader ";
    msg += fullPath;
    logger.info(msg.c_str());
    return shader;
}

SDL_GPUShader* GCGameEngine::Shader::getShader(){
    return this->shader;
}