#include "Renderer/VBO.h"

GCGameEngine::VBO::VBO(SDL_GPUDevice* device){
    this->device = device;

    vertices = new Vertex[3];
    vertices[0] = Vertex{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    vertices[1] = Vertex{-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    vertices[2] = Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};

    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(vertices);
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertexBuffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(vertices);
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    memcpy(data, vertices, sizeof(Vertex) * 3);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transferBuffer;
    location.offset = 0;
    
    SDL_GPUBufferRegion region;
    region.buffer = vertexBuffer;
    region.size = sizeof(vertices);
    region.offset = 0;

    SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);
}   

GCGameEngine::VBO::~VBO(){
    delete[] vertices;
    SDL_ReleaseGPUBuffer(device, vertexBuffer); 
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
}