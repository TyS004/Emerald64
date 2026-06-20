#include "Renderer/SDLRenderer.h"
#include "Renderer/SDLGPURegistry.h"
#include <Window/SDLWindow.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

E64::SDLRenderer::SDLRenderer(){
    SDLWindow* sdl_window = dynamic_cast<E64::SDLWindow*>(E64::Engine::ctx->window);
    window = sdl_window->getWindow();
    device = sdl_window->getDevice();
    width =  sdl_window->getWidth();
    height = sdl_window->getHeight();

    depth_texture_info = {};
    depth_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    depth_texture_info.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT_S8_UINT;
    depth_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    depth_texture_info.width  = width;
    depth_texture_info.height = height;
    depth_texture_info.layer_count_or_depth = 1;
    depth_texture_info.num_levels = 1;
    depth_texture_info.sample_count = SDL_GPU_SAMPLECOUNT_1;
    depth_texture = SDL_CreateGPUTexture(device, &depth_texture_info);

    scene_texture_info = {};
    scene_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    scene_texture_info.format = SDL_GetGPUSwapchainTextureFormat(device, window);
    scene_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    scene_texture_info.width  = width;
    scene_texture_info.height = height;
    scene_texture_info.layer_count_or_depth = 1;
    scene_texture_info.num_levels = 1;
    scene_texture = SDL_CreateGPUTexture(device, &scene_texture_info);

    depth_target_info = {};
    depth_target_info.clear_depth = 1.0f;
    depth_target_info.texture = depth_texture;
    depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    depth_target_info.store_op = SDL_GPU_STOREOP_STORE;

    color_target_info = {};
    color_target_info.clear_color = {75/255.0f, 75/255.0f, 75/255.0f, 255/255.0f};
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;

    draw_calls = 0;

    pipelines.push_back(std::make_unique<SDLPipeline>("../assets/shaders/object"));
    pipelines.push_back(std::make_unique<SDLPipeline>("../assets/shaders/object", SDL_GPU_FILLMODE_LINE));
    pipelines[0].get()->setStencilFrontCompareOP(SDL_GPU_COMPAREOP_ALWAYS);
    pipelines[0].get()->setStencilFrontPassOP(SDL_GPU_STENCILOP_REPLACE);
    pipelines[0].get()->buildPipeline();
    pipelines[1].get()->setStencilFrontCompareOP(SDL_GPU_COMPAREOP_NOT_EQUAL);
    pipelines[1].get()->setStencilFrontPassOP(SDL_GPU_STENCILOP_KEEP);
    pipelines[1].get()->buildPipeline();

    stbi_set_flip_vertically_on_load(true);
}

E64::SDLRenderer::~SDLRenderer(){

}

void E64::SDLRenderer::startFrame(){
    cmd_buf = SDL_AcquireGPUCommandBuffer(device);
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buf, window, &swapchain, &width, &height);
    if (!swapchain) {
        SDL_CancelGPUCommandBuffer(cmd_buf);
        cmd_buf = nullptr;
    }
}

void E64::SDLRenderer::OnImGuiResize(float width, float height){
    pending_resize = true;
    this->width = width;
    this->height = height;
}

void E64::SDLRenderer::ResizeViewport(){
    if(width == 0 || height == 0) return;

    if (depth_texture)
        SDL_ReleaseGPUTexture(device, depth_texture);
    if (scene_texture)
        SDL_ReleaseGPUTexture(device, scene_texture);

    depth_texture_info.width  = width;
    depth_texture_info.height = height; 
    depth_texture = SDL_CreateGPUTexture(device, &depth_texture_info);

    scene_texture_info.width  = width;
    scene_texture_info.height = height;
    scene_texture = SDL_CreateGPUTexture(device, &scene_texture_info);
}

void E64::SDLRenderer::beginRenderPass(E64::RenderTarget target){
    draw_calls = 0;
    current_render_pass++;

    this->target = target;
    switch(target){
        case E64::SWAPCHAIN:
            color_target_info.texture = swapchain;
            render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, &depth_target_info);
            break;
        case E64::TEXTURE:
            color_target_info.texture = scene_texture;
            render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, &depth_target_info);
            break;
    }
}

void E64::SDLRenderer::endRenderPass(){
    SDL_EndGPURenderPass(render_pass);
}

void E64::SDLRenderer::bindPipeline(){
    SDLPipeline* pipeline = pipelines[current_render_pass - 1].get();
    SDL_BindGPUGraphicsPipeline(render_pass, pipeline->getPipeline());
}

void E64::SDLRenderer::bindVertexBuffers(E64::ECS::MeshComponent* comp){
    E64::Mesh* mesh = E64::Engine::ctx->asset_manager->getMeshAsset(comp->mesh_handle);

    SDL_GPUBufferBinding buffer_binding = {};
    buffer_binding.buffer = SDLGPURegistry::vbo_registry.at(mesh->vbo);
    buffer_binding.offset = 0;
    SDL_BindGPUVertexBuffers(render_pass, 0, &buffer_binding, 1);
}

void E64::SDLRenderer::bindIndexBuffers(E64::ECS::MeshComponent* comp){
    E64::Mesh* mesh = E64::Engine::ctx->asset_manager->getMeshAsset(comp->mesh_handle);

    SDL_GPUBuffer* index_buffer = SDLGPURegistry::ibo_registry.at(mesh->ibo);
    SDL_GPUBufferBinding binding;
    binding.buffer = index_buffer;
    binding.offset = 0;

    SDL_BindGPUIndexBuffer(render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
}

void E64::SDLRenderer::bindFragmentSamplers(E64::ECS::MeshComponent* comp){
    E64::AssetManager* assetmanager = E64::Engine::ctx->asset_manager.get();
    E64::Texture* texture = assetmanager->getTextureAsset(comp->tex_handle);

    SDL_GPUTextureSamplerBinding binding;
    binding.texture = SDLGPURegistry::texture_registry[texture->texture];
    binding.sampler = SDLGPURegistry::sampler_registry[texture->sampler];

    SDL_BindGPUFragmentSamplers(render_pass, 0, &binding, 1);
}

void E64::SDLRenderer::draw(E64::ECS::MeshComponent* comp){
    E64::Mesh* mesh = E64::Engine::ctx->asset_manager->getMeshAsset(comp->mesh_handle);
    if(!mesh) { E64::Log::error("MESH IS NULLPTR"); return; }

    bindVertexBuffers(comp);
    bindIndexBuffers(comp);
    bindFragmentSamplers(comp);
    
    draw_calls++;
    SDL_DrawGPUIndexedPrimitives(render_pass, mesh->indices.size(), 1, 0, 0, 0);
}

E64::GPUBufferHandle E64::SDLRenderer::createVertexBuffer(std::vector<E64::Vertex> vertices){
    SDL_GPUBufferCreateInfo bufferInfo{};
    bufferInfo.size = sizeof(E64::Vertex) * vertices.size();
    bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    SDL_GPUBuffer* vertex_buffer = SDL_CreateGPUBuffer(device, &bufferInfo);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = sizeof(E64::Vertex) * vertices.size();
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    E64::Vertex* data = (E64::Vertex*)SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, vertices.data(), sizeof(E64::Vertex) * vertices.size());
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

    SDL_GPUTransferBufferLocation location{};
    location.transfer_buffer = transfer_buffer;
    location.offset = 0;
    
    SDL_GPUBufferRegion region;
    region.buffer = vertex_buffer;
    region.size = sizeof(E64::Vertex) * vertices.size();
    region.offset = 0;

    SDL_UploadToGPUBuffer(copyPass, &location, &region, true);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);

    E64::GPUBufferHandle handle = SDLGPURegistry::vbo_handle;
    SDLGPURegistry::vbo_registry[SDLGPURegistry::vbo_handle++] = vertex_buffer;
    E64::Log::debug("Sent VBO Info to GPU");

    return handle;
}

E64::GPUBufferHandle E64::SDLRenderer::createIndexBuffer(std::vector<E64::Index> indices){
    SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);

    SDL_GPUBufferCreateInfo buffer_info{};
    buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    buffer_info.size = sizeof(uint32_t) * indices.size();
    SDL_GPUBuffer* index_buffer = SDL_CreateGPUBuffer(device, &buffer_info);

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

    E64::GPUBufferHandle handle = SDLGPURegistry::ibo_handle;
    SDLGPURegistry::ibo_registry[SDLGPURegistry::ibo_handle++] = index_buffer;
    E64::Log::debug("Sent IBO Info to GPU");
    
    return handle;
}

E64::GPUTextureHandle E64::SDLRenderer::createTexture(unsigned char* img_data, int width, int height){
    int img_size = width * height * 4;

    SDL_GPUTextureCreateInfo info = {};
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    info.width  = width;
    info.height = height;
    info.type   = SDL_GPU_TEXTURETYPE_2D;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &info);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = img_size;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    SDL_GPUTextureTransferInfo textureTransferInfo{};
    textureTransferInfo.offset = 0;
    textureTransferInfo.pixels_per_row = width;
    textureTransferInfo.rows_per_layer = height;
    textureTransferInfo.transfer_buffer = transfer_buffer;

    void* data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, img_data, img_size);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd_buffer);

    SDL_GPUTextureRegion region{};
    region.texture = texture;
    region.w = width;
    region.h = height;
    region.d = 1;
    region.x = 0;
    region.y = 0;
    region.z = 0;
    
    SDL_UploadToGPUTexture(copy_pass, &textureTransferInfo, &region, true);
    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(cmd_buffer);

    E64::GPUTextureHandle handle = SDLGPURegistry::texture_handle;
    SDLGPURegistry::texture_registry[SDLGPURegistry::texture_handle++] = texture;
    E64::Log::debug("Sent Texture Info to GPU");
    
    return handle;

    if (!texture)         { E64::Log::error(SDL_GetError()); }
    if (!transfer_buffer) { E64::Log::error(SDL_GetError()); }
}

E64::GPUSamplerHandle E64::SDLRenderer::createSampler(){
    SDL_GPUSamplerCreateInfo sampler_info{};
    sampler_info.min_filter = SDL_GPU_FILTER_LINEAR;          // For minification
    sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR;          // For magnification
    sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;

    SDL_GPUSampler* sampler = SDL_CreateGPUSampler(device, &sampler_info);

    E64::GPUSamplerHandle handle = SDLGPURegistry::sampler_handle;
    SDLGPURegistry::sampler_registry[SDLGPURegistry::sampler_handle++] = sampler;
    E64::Log::debug("Sent Sampler Info to GPU");
    
    return handle;
}

void E64::SDLRenderer::pushVertexUniform(const void* data, size_t size, uint32_t slot){
    SDL_PushGPUVertexUniformData(cmd_buf, slot, data, size);
}

void E64::SDLRenderer::pushFragmentUniform(const void* data, size_t size, uint32_t slot){
    SDL_PushGPUFragmentUniformData(cmd_buf, slot, data, size);
}

void E64::SDLRenderer::submit(){
    SDL_SubmitGPUCommandBuffer(cmd_buf);
    current_render_pass = 0;
}

void E64::SDLRenderer::drawUI(){
    ImGui_ImplSDLGPU3_RenderDrawData(ImGui::GetDrawData(), cmd_buf, render_pass);
}

SDL_GPUCommandBuffer* E64::SDLRenderer::getCommandBuffer(){
    return cmd_buf;
}

SDL_GPUTexture* E64::SDLRenderer::getSceneTexture(){
    return scene_texture;
}

void E64::SDLRenderer::setColorLoadOP(E64::RenderLoadOP OP){
    switch(OP){
        case E64::RenderLoadOP::CLEAR:
            this->color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            break;
        case E64::RenderLoadOP::LOAD:
            this->color_target_info.load_op = SDL_GPU_LOADOP_LOAD;
            break;
    }
}

void E64::SDLRenderer::setColorStoreOP(E64::RenderStoreOP OP){
    switch(OP){
        case E64::RenderStoreOP::STORE:
            this->color_target_info.store_op = SDL_GPU_STOREOP_STORE;
            break;
        case E64::RenderStoreOP::RESOLVE:
            this->color_target_info.store_op = SDL_GPU_STOREOP_RESOLVE;
            break;
    }
}

void E64::SDLRenderer::setDepthLoadOP(E64::RenderLoadOP OP){
    switch(OP){
        case E64::RenderLoadOP::CLEAR:
            this->depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            break;
        case E64::RenderLoadOP::LOAD:
            this->depth_target_info.load_op = SDL_GPU_LOADOP_LOAD;
            break;
    }
}

void E64::SDLRenderer::setDepthStoreOP(E64::RenderStoreOP OP){
    switch(OP){
        case E64::RenderStoreOP::STORE:
            this->depth_target_info.store_op = SDL_GPU_STOREOP_STORE;
            break;
        case E64::RenderStoreOP::RESOLVE:
            this->depth_target_info.store_op = SDL_GPU_STOREOP_RESOLVE;
            break;
    }
}

void E64::SDLRenderer::setStencilReference(int ref){
    SDL_SetGPUStencilReference(render_pass, ref);
}

int E64::SDLRenderer::getDrawCalls(){
    return draw_calls;
}

bool E64::SDLRenderer::isPendingResize(){
    return pending_resize;
}