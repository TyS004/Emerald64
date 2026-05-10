#include "Renderer/Pipeline.h"
#include "Logger/Logger.h"
#include "Renderer/VBO.h"

#include <cstddef>

static E64::Log logger = E64::Log();

E64::Pipeline::Pipeline(const char* shaderPath){
    this->device = E64::Window::getDevice();
    SDL_Window* window = E64::Window::getWindow();
    
    vert_shader = new Shader(shaderPath, SDL_GPU_SHADERSTAGE_VERTEX, device);
    frag_shader = new Shader(shaderPath, SDL_GPU_SHADERSTAGE_FRAGMENT, device);

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.vertex_shader = vert_shader->getShader();
    pipelineInfo.fragment_shader = frag_shader->getShader();

    SDL_GPUDepthStencilState stencil_state = {};
    stencil_state.enable_depth_test = true;
    stencil_state.enable_depth_write = true;
    stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;

    pipelineInfo.depth_stencil_state = stencil_state;
    pipelineInfo.target_info.has_depth_stencil_target = true;
    pipelineInfo.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    
    SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
    vertexBufferDesctiptions[0].slot = 0;
    vertexBufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertexBufferDesctiptions[0].instance_step_rate = 0;
    vertexBufferDesctiptions[0].pitch = sizeof(Vertex);

    pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
    pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

    SDL_GPUVertexAttribute vertexAttributes[2];
    // a_position
    vertexAttributes[0].buffer_slot = 0; // fetch data from the buffer at slot 0
    vertexAttributes[0].location = 0; // layout (location = 0) in shader
    vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3; //vec3
    vertexAttributes[0].offset = 0; // start from the first byte from current buffer position

    // a_color
    vertexAttributes[1].buffer_slot = 0; // use buffer at slot 0
    vertexAttributes[1].location = 1; // layout (location = 1) in shader
    vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4; // vec4
    vertexAttributes[1].offset = offsetof(Vertex, color); // 4th float from current buffer position

    pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
    pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

    SDL_GPUTextureFormat fmt = SDL_GetGPUSwapchainTextureFormat(device, window);
    SDL_GPUColorTargetDescription colorTargetDesc = {};
    colorTargetDesc.format = fmt;
    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;

    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

E64::Pipeline::~Pipeline(){
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

SDL_GPUGraphicsPipeline* E64::Pipeline::getPipeline(){
    return this->pipeline;
}