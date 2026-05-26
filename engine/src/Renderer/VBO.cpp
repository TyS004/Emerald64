#include "Renderer/VBO.h"
#include "Window/Window.h"

E64::VBO::VBO(){
    //DEFAULT TRIANGLE
    this->device = E64::Window::getDevice();
    this->num_vertices = 36;
    this->vertices = std::vector<Vertex>(this->num_vertices);

    // Front (red)
    vertices[0]  = {{-0.5f, -0.5f,  0.5f}, {1,0,0,1}, {0,0}};
    vertices[1]  = {{ 0.5f, -0.5f,  0.5f}, {1,0,0,1}, {1,0}};
    vertices[2]  = {{ 0.5f,  0.5f,  0.5f}, {1,0,0,1}, {1,1}};
    vertices[3]  = {{-0.5f, -0.5f,  0.5f}, {1,0,0,1}, {0,0}};
    vertices[4]  = {{ 0.5f,  0.5f,  0.5f}, {1,0,0,1}, {1,1}};
    vertices[5]  = {{-0.5f,  0.5f,  0.5f}, {1,0,0,1}, {0,1}};

    // Back (green)
    vertices[6]  = {{ 0.5f, -0.5f, -0.5f}, {0,1,0,1}, {0,0}};
    vertices[7]  = {{-0.5f, -0.5f, -0.5f}, {0,1,0,1}, {1,0}};
    vertices[8]  = {{-0.5f,  0.5f, -0.5f}, {0,1,0,1}, {1,1}};
    vertices[9]  = {{ 0.5f, -0.5f, -0.5f}, {0,1,0,1}, {0,0}};
    vertices[10] = {{-0.5f,  0.5f, -0.5f}, {0,1,0,1}, {1,1}};
    vertices[11] = {{ 0.5f,  0.5f, -0.5f}, {0,1,0,1}, {0,1}};

    // Left (blue)
    vertices[12] = {{-0.5f, -0.5f, -0.5f}, {0,0,1,1}, {0,0}};
    vertices[13] = {{-0.5f, -0.5f,  0.5f}, {0,0,1,1}, {1,0}};
    vertices[14] = {{-0.5f,  0.5f,  0.5f}, {0,0,1,1}, {1,1}};
    vertices[15] = {{-0.5f, -0.5f, -0.5f}, {0,0,1,1}, {0,0}};
    vertices[16] = {{-0.5f,  0.5f,  0.5f}, {0,0,1,1}, {1,1}};
    vertices[17] = {{-0.5f,  0.5f, -0.5f}, {0,0,1,1}, {0,1}};

    // Right (yellow)
    vertices[18] = {{ 0.5f, -0.5f,  0.5f}, {1,1,0,1}, {0,0}};
    vertices[19] = {{ 0.5f, -0.5f, -0.5f}, {1,1,0,1}, {1,0}};
    vertices[20] = {{ 0.5f,  0.5f, -0.5f}, {1,1,0,1}, {1,1}};
    vertices[21] = {{ 0.5f, -0.5f,  0.5f}, {1,1,0,1}, {0,0}};
    vertices[22] = {{ 0.5f,  0.5f, -0.5f}, {1,1,0,1}, {1,1}};
    vertices[23] = {{ 0.5f,  0.5f,  0.5f}, {1,1,0,1}, {0,1}};

    // Top (magenta)
    vertices[24] = {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}};
    vertices[25] = {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,0}};
    vertices[26] = {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}};
    vertices[27] = {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}};
    vertices[28] = {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}};
    vertices[29] = {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {0,1}};

    // Bottom (cyan)
    vertices[30] = {{-0.5f, -0.5f, -0.5f}, {0,1,1,1}, {0,0}};
    vertices[31] = {{ 0.5f, -0.5f, -0.5f}, {0,1,1,1}, {1,0}};
    vertices[32] = {{ 0.5f, -0.5f,  0.5f}, {0,1,1,1}, {1,1}};
    vertices[33] = {{-0.5f, -0.5f, -0.5f}, {0,1,1,1}, {0,0}};
    vertices[34] = {{ 0.5f, -0.5f,  0.5f}, {0,1,1,1}, {1,1}};
    vertices[35] = {{-0.5f, -0.5f,  0.5f}, {0,1,1,1}, {0,1}};
}

E64::VBO::VBO(std::vector<Vertex> vertices){
    this->device = E64::Window::getDevice();
    this->num_vertices = vertices.size();
    this->vertices = vertices;
}   

E64::VBO::~VBO(){

}

void E64::VBO::upload(){
    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(Vertex) * num_vertices;
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(Vertex) * num_vertices;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    Vertex* data = (Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, vertices.data(), sizeof(Vertex) * num_vertices);
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

    E64::Log::debug("Sent VBO Info to GPU");
}

SDL_GPUBufferBinding* E64::VBO::getBufferBinding(){
    return &buffer_binding;
}

int E64::VBO::getNumVertices(){
    return this->num_vertices;
}