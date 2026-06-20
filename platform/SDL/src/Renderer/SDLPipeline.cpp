#include "Renderer/SDLPipeline.h"
#include "Renderer/SDLShader.h"
#include <Window/SDLWindow.h>

#include <cstddef>

E64::SDLPipeline::SDLPipeline(const char* shaderPath, SDL_GPUFillMode fill_mode){
    SDLWindow* sdl_window = dynamic_cast<E64::SDLWindow*>(E64::Engine::ctx->window);
    this->device = sdl_window->getDevice();
    SDL_Window* window = sdl_window->getWindow();
    
    vert_shader = new SDLShader(shaderPath, SDL_GPU_SHADERSTAGE_VERTEX, device);
    frag_shader = new SDLShader(shaderPath, SDL_GPU_SHADERSTAGE_FRAGMENT, device);

    pipelineInfo = {};
    pipelineInfo.vertex_shader = vert_shader->getShader();
    pipelineInfo.fragment_shader = frag_shader->getShader();

    //BUG WHEN NO CULLING
    pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;

    this->fill_mode = fill_mode;
    pipelineInfo.rasterizer_state.fill_mode = fill_mode;

    stencil_state = {};
    stencil_state.enable_depth_test = true;
    stencil_state.enable_depth_write = true;
    stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;
    stencil_state.enable_stencil_test = true;

    stencil_state.compare_mask = 0xFF;
    stencil_state.write_mask = 0xFF;
    
    stencil_state.front_stencil_state.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
    stencil_state.front_stencil_state.pass_op = SDL_GPU_STENCILOP_REPLACE;
    stencil_state.front_stencil_state.fail_op = SDL_GPU_STENCILOP_KEEP;
    stencil_state.front_stencil_state.depth_fail_op = SDL_GPU_STENCILOP_KEEP;

    stencil_state.back_stencil_state.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
    stencil_state.back_stencil_state.pass_op = SDL_GPU_STENCILOP_REPLACE;
    stencil_state.back_stencil_state.fail_op = SDL_GPU_STENCILOP_KEEP;
    stencil_state.back_stencil_state.depth_fail_op = SDL_GPU_STENCILOP_KEEP;

    pipelineInfo.depth_stencil_state = stencil_state;
    pipelineInfo.target_info.has_depth_stencil_target = true;
    pipelineInfo.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT;

    vertexBufferDesctiptions[0].slot = 0;
    vertexBufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    vertexBufferDesctiptions[0].instance_step_rate = 0;
    vertexBufferDesctiptions[0].pitch = sizeof(E64::Vertex);

    pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
    pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

    // a_position
    vertexAttributes[0].buffer_slot = 0; // fetch data from the buffer at slot 0
    vertexAttributes[0].location = 0; // layout (location = 0) in shader
    vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3; // vec3
    vertexAttributes[0].offset = 0; // start from the first byte from current buffer position

    // a_color
    vertexAttributes[1].buffer_slot = 0; // use buffer at slot 0
    vertexAttributes[1].location = 1; // layout (location = 1) in shader
    vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4; // vec4
    vertexAttributes[1].offset = offsetof(E64::Vertex, color); // 4th float from current buffer position

    //a_uv
    vertexAttributes[2].buffer_slot = 0;
    vertexAttributes[2].location = 2; // layout (location = 2) in shader
    vertexAttributes[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2; // vec2
    vertexAttributes[2].offset = offsetof(E64::Vertex, uv);    // 4th float from current buffer position

    //a_norm
    vertexAttributes[3].buffer_slot = 0;
    vertexAttributes[3].location = 3; // layout (location = 3) in shader
    vertexAttributes[3].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3; // vec2
    vertexAttributes[3].offset = offsetof(E64::Vertex, norm);    // 4th float from current buffer position

    pipelineInfo.vertex_input_state.num_vertex_attributes = 4;
    pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

    fmt = SDL_GetGPUSwapchainTextureFormat(device, window);
    colorTargetDesc = {};
    colorTargetDesc.format = fmt;
    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;

    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    this->pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

E64::SDLPipeline::~SDLPipeline(){
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

SDL_GPUGraphicsPipeline* E64::SDLPipeline::getPipeline(){
    return this->pipeline;
}

void E64::SDLPipeline::setStencilFrontCompareOP(SDL_GPUCompareOp OP){
    this->stencil_state.front_stencil_state.compare_op = OP;
}

void E64::SDLPipeline::setStencilFrontPassOP(SDL_GPUStencilOp OP){
    this->stencil_state.front_stencil_state.pass_op = OP;
}

void E64::SDLPipeline::buildPipeline(){
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    this->pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

SDL_GPUFillMode E64::SDLPipeline::getFillMode(){
    return this->fill_mode;
}