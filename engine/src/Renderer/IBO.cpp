#include "Renderer/IBO.h"

GCGameEngine::IBO::IBO(){
    //DEFAULT TRIANGLE
    
    num_indices = 3;
    
    indices = new uint32_t[num_indices];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    sendToGPU();
}

GCGameEngine::IBO::IBO(uint32_t* indices, int num_indices){
    this->num_indices = num_indices;
    this->indices = indices;

    sendToGPU();
}

void GCGameEngine::IBO::sendToGPU(){
    SDL_GPUDevice* device = GCGameEngine::Window::getDevice();
    SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUBufferCreateInfo buffer_info{};
    buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    buffer_info.size = sizeof(uint32_t) * num_indices;
    index_buffer = SDL_CreateGPUBuffer(device, &buffer_info);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(uint32_t) * num_indices;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    uint32_t* data = (uint32_t*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, indices, sizeof(uint32_t) * num_indices);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd_buffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;

    SDL_GPUBufferRegion region;
    region.buffer = index_buffer;
    region.size = sizeof(uint32_t) * num_indices;
    region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &location, &region, true);
    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(cmd_buffer);
}

GCGameEngine::IBO::~IBO(){
    delete[] indices;
}

SDL_GPUBuffer* GCGameEngine::IBO::getIndexBuffer(){
    return index_buffer;
}

int GCGameEngine::IBO::getNumIndices(){
    return num_indices;
}