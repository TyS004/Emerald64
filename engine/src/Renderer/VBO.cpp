#include "Renderer/VBO.h"

GCGameEngine::VBO::VBO(SDL_GPUDevice* device, const uint16_t num_vertices){
    this->device = device;
    this->num_vertices = num_vertices;

    vertices = new Vertex[3];
    vertices[0] = Vertex{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    vertices[1] = Vertex{-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    vertices[2] = Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};

    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(Vertex) * 3;
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(Vertex) * 3;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, vertices, sizeof(Vertex) * 3);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;
    
    SDL_GPUBufferRegion region;
    region.buffer = vertex_buffer;
    region.size = sizeof(Vertex) * 3;
    region.offset = 0;

    m_buffer_binding = new SDL_GPUBufferBinding;
    m_buffer_binding->buffer = vertex_buffer;
    m_buffer_binding->offset = 0;

    SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);
}   

GCGameEngine::VBO::~VBO(){
    delete[] vertices;
    SDL_ReleaseGPUBuffer(device, vertex_buffer); 
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

SDL_GPUBufferBinding* GCGameEngine::VBO::getBufferBinding(){
    return m_buffer_binding;
}