#include "Renderer/SDLPipeline.h"
#include "Renderer/SDLShader.h"
#include <Window/SDLWindow.h>

#include <cstddef>

E64::SDLPipeline::SDLPipeline(const char* shaderPath, int vert_uniforms, int frag_uniforms, SDL_GPUFillMode fill_mode){
    SDLWindow* sdl_window = dynamic_cast<E64::SDLWindow*>(E64::Engine::ctx->window);
    this->device = sdl_window->getDevice();
    SDL_Window* window = sdl_window->getWindow();
    
    vert_shader = new SDLShader(shaderPath, SDL_GPU_SHADERSTAGE_VERTEX, device, vert_uniforms);
    frag_shader = new SDLShader(shaderPath, SDL_GPU_SHADERSTAGE_FRAGMENT, device, frag_uniforms);

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
    stencil_state.enable_stencil_test = false;

    stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;

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
    colorTargetDesc.blend_state.enable_blend = true;
    colorTargetDesc.blend_state.src_color_blendfactor =
        SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    colorTargetDesc.blend_state.dst_color_blendfactor =
        SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    colorTargetDesc.blend_state.color_blend_op =
        SDL_GPU_BLENDOP_ADD;
    colorTargetDesc.blend_state.src_alpha_blendfactor =
        SDL_GPU_BLENDFACTOR_ONE;
    colorTargetDesc.blend_state.dst_alpha_blendfactor =
        SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    colorTargetDesc.blend_state.alpha_blend_op =
        SDL_GPU_BLENDOP_ADD;
    pipelineInfo.target_info.num_color_targets = 1;
    pipelineInfo.target_info.color_target_descriptions = &colorTargetDesc;

    pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

    this->pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

E64::SDLPipeline::SDLPipeline(const char* vertexShaderPath, const char* fragmentShaderPath, int vert_uniforms, int frag_uniforms, SDL_GPUFillMode fill_mode) {
    SDLWindow* sdl_window = dynamic_cast<E64::SDLWindow*>(E64::Engine::ctx->window);
    this->device = sdl_window->getDevice();
    SDL_Window* window = sdl_window->getWindow();

    vert_shader = new SDLShader(vertexShaderPath, SDL_GPU_SHADERSTAGE_VERTEX, device, vert_uniforms);
    frag_shader = new SDLShader(fragmentShaderPath, SDL_GPU_SHADERSTAGE_FRAGMENT, device, frag_uniforms);

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
    stencil_state.enable_stencil_test = true;

    stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL;

    stencil_state.compare_mask = 0xFF;
    stencil_state.write_mask   = 0xFF;

    SDL_GPUStencilOpState write_state{};
    write_state.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
    write_state.pass_op = SDL_GPU_STENCILOP_REPLACE;
    write_state.fail_op = SDL_GPU_STENCILOP_KEEP;
    write_state.depth_fail_op = SDL_GPU_STENCILOP_KEEP;

    stencil_state.front_stencil_state = write_state;
    stencil_state.back_stencil_state = write_state;

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

SDL_GPUDepthStencilState* E64::SDLPipeline::getDepthStencilState() {
    return &this->stencil_state;
}

SDL_GPUGraphicsPipelineCreateInfo* E64::SDLPipeline::getPipelineInfo() {
    return &this->pipelineInfo;
}

void E64::SDLPipeline::buildPipeline(){
    pipelineInfo.depth_stencil_state = stencil_state;
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    this->pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
}

SDL_GPUFillMode E64::SDLPipeline::getFillMode(){
    return this->fill_mode;
}