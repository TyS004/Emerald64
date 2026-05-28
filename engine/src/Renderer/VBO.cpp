#include "Renderer/VBO.h"
#include "Window/Window.h"

E64::VBO::VBO(){
    
}

E64::VBO::~VBO(){

}

void E64::VBO::upload(std::vector<Vertex> vertices){
    SDL_GPUDevice* device = E64::Window::getDevice();

    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(Vertex) * vertices.size();
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(Vertex) * vertices.size();
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;
    
    SDL_GPUBufferRegion region;
    region.buffer = vertex_buffer;
    region.size = sizeof(Vertex) * vertices.size();
    region.offset = 0;

    SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);

    E64::Log::debug("Sent VBO Info to GPU");
}

SDL_GPUBuffer* E64::VBO::getVertexBuffer(){
    return vertex_buffer;
}