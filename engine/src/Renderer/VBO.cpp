#include "Renderer/VBO.h"
#include "Window/Window.h"

GCGameEngine::VBO::VBO(const uint16_t num_vertices){
    this->device = GCGameEngine::Window::getDevice();
    this->num_vertices = num_vertices;

    vec3 pos1 = {0.0f, 0.5f, 0.0f};
    vec3 pos2 = {-0.5f, -0.5f, 0.0f};
    vec3 pos3 = {0.5f, -0.5f, 0.0f};

    vec4 color1 = {1.0f, 0.0f, 1.0f, 1.0f};
    vec4 color2 = {1.0f, 1.0f, 0.0f, 1.0f};
    vec4 color3 = {1.0f, 0.0f, 0.25f, 1.0f};

    vertices = new Vertex[3];
    vertices[0] = Vertex{pos1, color1};
    vertices[1] = Vertex{pos2, color2};
    vertices[2] = Vertex{pos3, color3};

    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(Vertex) * num_vertices;
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(Vertex) * num_vertices;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, vertices, sizeof(Vertex) * num_vertices);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;
    
    SDL_GPUBufferRegion region;
    region.buffer = vertex_buffer;
    region.size = sizeof(Vertex) * num_vertices;
    region.offset = 0;

    buffer_binding = {};
    buffer_binding.buffer = vertex_buffer;
    buffer_binding.offset = 0;

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
    return &buffer_binding;
}