#include "Renderer/SDLShader.h"
#include <Logger/Logger.h>

#include <string>

E64::SDLShader::SDLShader(const char* path, SDL_GPUShaderStage stage, SDL_GPUDevice* device){
    this->device = device;
    this->stage = stage;
    this->path = path;
    this->shader = loadShader();
}

E64::SDLShader::~SDLShader(){
    SDL_ReleaseGPUShader(this->device, this->shader);
}

SDL_GPUShader* E64::SDLShader::loadShader(){
    char fullPath[256];
    if(stage == SDL_GPU_SHADERSTAGE_VERTEX){
        #ifdef E64_APPLE
            snprintf(fullPath, sizeof(fullPath), "%s_vert.metal", path);
        #else
            snprintf(fullPath, sizeof(fullPath), "%s_vert.dxil", path);
        #endif
    }
    else{
        #ifdef E64_APPLE
            snprintf(fullPath, sizeof(fullPath), "%s_frag.metal", path);
        #else
            snprintf(fullPath, sizeof(fullPath), "%s_frag.dxil", path);
        #endif
    }

    size_t codeSize;
    void* code = SDL_LoadFile(fullPath, &codeSize);
    if(!code){  
        E64::Log::warn("Failed to Load Shader File");
        E64::Log::warn(fullPath);
        return nullptr;
    }

    SDL_GPUShaderCreateInfo info = {};
    info.code = (const Uint8*) code;
    info.code_size = codeSize;
    info.stage = stage;
    info.num_storage_buffers = 0;
    info.num_storage_textures = 0;

    #ifdef E64_APPLE
        info.format = SDL_GPU_SHADERFORMAT_MSL;
    #else
        info.format = SDL_GPU_SHADERFORMAT_DXIL;
    #endif

    if(stage == SDL_GPU_SHADERSTAGE_VERTEX){
        info.num_uniform_buffers = 4;
        info.num_samplers = 0;
        #ifdef E64_APPLE
                info.entrypoint = "vertex_main";
        #else
                info.entrypoint = "main";
        #endif 
    }
    else{
        info.num_uniform_buffers = 0;
        info.num_samplers = 1;
        #ifdef E64_APPLE
                info.entrypoint = "fragment_main";
        #else
                info.entrypoint = "main";
        #endif 
    }

    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
    SDL_free(code);

    if(!shader){
        E64::Log::warn("Failed to Create Shader");
        return nullptr;
    }
    // std::string msg = "Successfully Loaded Shader ";
    // msg += fullPath;
    // E64::Log::info(msg.c_str());
    return shader;
}

SDL_GPUShader* E64::SDLShader::getShader(){
    return this->shader;
}