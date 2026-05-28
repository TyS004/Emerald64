#include "Renderer/Renderer.h"
#include "Layer/Layer.h"

E64::Renderer::Renderer(){
    depth_texture_info = {};
    depth_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    depth_texture_info.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    depth_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    depth_texture_info.width  = E64::Window::getWidth();
    depth_texture_info.height = E64::Window::getHeight();
    depth_texture_info.layer_count_or_depth = 1;
    depth_texture_info.num_levels = 1;
    depth_texture_info.sample_count = SDL_GPU_SAMPLECOUNT_1;
    depth_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &depth_texture_info);

    scene_texture_info = {};
    scene_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    scene_texture_info.format = SDL_GetGPUSwapchainTextureFormat(E64::Window::getDevice(), E64::Window::getWindow());
    scene_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    scene_texture_info.width  = E64::Window::getWidth();
    scene_texture_info.height = E64::Window::getHeight();
    scene_texture_info.layer_count_or_depth = 1;
    scene_texture_info.num_levels = 1;
    scene_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &scene_texture_info);

    depth_target_info = {};
    color_target_info = {};

    width = E64::Window::getWidth();
    height = E64::Window::getHeight();

    draw_calls = 0;

    pipeline = std::make_unique<Pipeline>("../assets/shaders/object");
}

E64::Renderer::~Renderer(){

}

void E64::Renderer::aquireCmdBufferandSwapChain(){
    cmd_buf = SDL_AcquireGPUCommandBuffer(E64::Window::getDevice());
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buf, E64::Window::getWindow(), &swapchain, &width, &height);
    if (!swapchain) {
        SDL_CancelGPUCommandBuffer(cmd_buf);
        cmd_buf = nullptr;
    }
}

void E64::Renderer::OnImGuiResize(float width, float height){
    pending_resize = true;
    this->width = width;
    this->height = height;
}

void E64::Renderer::ResizeViewport(){
    if(width == 0 || height == 0) return;

    if (depth_texture)
        SDL_ReleaseGPUTexture(E64::Window::getDevice(), depth_texture);
    if (scene_texture)
        SDL_ReleaseGPUTexture(E64::Window::getDevice(), scene_texture);

    depth_texture_info.width  = width;
    depth_texture_info.height = height; 
    depth_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &depth_texture_info);

    scene_texture_info.width  = width;
    scene_texture_info.height = height;
    scene_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &scene_texture_info);
}

void E64::Renderer::beginRenderPass(RenderTarget target){
    draw_calls = 0;

    color_target_info.clear_color = {75/255.0f, 75/255.0f, 75/255.0f, 255/255.0f};
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;
    
    depth_target_info.clear_depth = 1.0f;
    depth_target_info.texture = depth_texture;
    depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    depth_target_info.store_op = SDL_GPU_STOREOP_STORE;

    switch(target){ 
        case SWAPCHAIN:
            color_target_info.texture = swapchain;
            render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, &depth_target_info);
            break;
        case TEXTURE:
            color_target_info.texture = scene_texture;
            render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, &depth_target_info);
            break;
    }
}

void E64::Renderer::endRenderPass(){
    SDL_EndGPURenderPass(render_pass);
}

void E64::Renderer::bindPipeline(){
    SDL_BindGPUGraphicsPipeline(render_pass, pipeline->getPipeline());
}

void E64::Renderer::bindVertexBuffers(E64::ECS::Mesh* mesh){
    if(!mesh) { E64::Log::error("MESH IS NULLPTR"); return; }

    SDL_GPUBufferBinding buffer_binding = {};
    buffer_binding.buffer = mesh->vbo.getVertexBuffer();
    buffer_binding.offset = 0;
    SDL_BindGPUVertexBuffers(render_pass, 0, &buffer_binding, 1);
}

void E64::Renderer::bindIndexBuffers(E64::ECS::Mesh* mesh){
    if(!mesh) { E64::Log::error("MESH IS NULLPTR"); return; }

    SDL_GPUBuffer* index_buffer = mesh->ibo.getIndexBuffer();
    SDL_GPUBufferBinding binding;
    binding.buffer = index_buffer;
    binding.offset = 0;

    SDL_BindGPUIndexBuffer(render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
}

void E64::Renderer::bindFragmentSamplers(E64::ECS::Mesh* mesh){
    if(!mesh) { E64::Log::error("MESH IS NULLPTR"); return; }

    SDL_GPUTextureSamplerBinding binding;
    binding.sampler = mesh->texture.getSampler();
    binding.texture= mesh->texture.getTexture();

    SDL_BindGPUFragmentSamplers(render_pass, 0, &binding, 1);
}

void E64::Renderer::draw(E64::ECS::Mesh* mesh){
    if(!mesh) { E64::Log::error("MESH IS NULLPTR"); return; }

    draw_calls++;
    SDL_DrawGPUIndexedPrimitives(render_pass, mesh->indices.size(), 1, 0, 0, 0);
}

void E64::Renderer::sendUniforms(glm::mat4 mvp){
    SDL_PushGPUVertexUniformData(cmd_buf, 0, &mvp, sizeof(mvp));
}

void E64::Renderer::submit(){
    SDL_SubmitGPUCommandBuffer(cmd_buf);
}

void E64::Renderer::drawUI(){
    ImGui_ImplSDLGPU3_RenderDrawData(ImGui::GetDrawData(), cmd_buf, render_pass);
}

SDL_GPURenderPass* E64::Renderer::getRenderPass(){
    return render_pass;
}

SDL_GPUCommandBuffer* E64::Renderer::getCommandBuffer(){
    return cmd_buf;
}

SDL_GPUTexture* E64::Renderer::getDepthTexture(){
    return depth_texture;
}

SDL_GPUTexture* E64::Renderer::getSceneTexture(){
    return scene_texture;
}

int E64::Renderer::getDrawCalls(){
    return draw_calls;
}

bool E64::Renderer::isPendingResize(){
    return pending_resize;
}