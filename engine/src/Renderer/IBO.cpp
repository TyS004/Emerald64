#include "Renderer/IBO.h"

E64::IBO::IBO(){

}

E64::IBO::~IBO(){
}

void E64::IBO::upload(std::vector<uint32_t> indices){
    SDL_GPUDevice* device = E64::Window::getDevice();
    SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUBufferCreateInfo buffer_info{};
    buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    buffer_info.size = sizeof(uint32_t) * indices.size();
    index_buffer = SDL_CreateGPUBuffer(device, &buffer_info);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(uint32_t) * indices.size();
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    uint32_t* data = (uint32_t*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, indices.data(), sizeof(uint32_t) * indices.size());
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd_buffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;

    SDL_GPUBufferRegion region;
    region.buffer = index_buffer;
    region.size = sizeof(uint32_t) * indices.size();
    region.offset = 0;

    SDL_UploadToGPUBuffer(copy_pass, &location, &region, true);
    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(cmd_buffer);
}

SDL_GPUBuffer* E64::IBO::getIndexBuffer(){
    return index_buffer;
}