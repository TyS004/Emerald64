#include "Renderer/Renderer.h"

E64::Renderer::Renderer(){
    depth_texture_info = {};
    depth_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    depth_texture_info.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    depth_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    depth_texture_info.width  = 800;
    depth_texture_info.height = 600;
    depth_texture_info.layer_count_or_depth = 1;
    depth_texture_info.num_levels = 1;
    depth_texture_info.sample_count = SDL_GPU_SAMPLECOUNT_1;
    depth_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &depth_texture_info);

    scene_texture_info = {};
    scene_texture_info.type   = SDL_GPU_TEXTURETYPE_2D;
    scene_texture_info.format = SDL_GetGPUSwapchainTextureFormat(E64::Window::getDevice(), E64::Window::getWindow());
    scene_texture_info.usage  = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    scene_texture_info.width  = 800;
    scene_texture_info.height = 600;
    scene_texture_info.layer_count_or_depth = 1;
    scene_texture_info.num_levels = 1;
    scene_texture = SDL_CreateGPUTexture(E64::Window::getDevice(), &scene_texture_info);

    width = 800;
    height = 600;
}

E64::Renderer::~Renderer(){

}

void E64::Renderer::OnResize(float width, float height){
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

void E64::Renderer::beginSceneRenderPass(){
    cmd_buf = SDL_AcquireGPUCommandBuffer(E64::Window::getDevice());

    color_target_info = {};
    color_target_info.clear_color = {0/255.0f, 0/255.0f, 0/255.0f, 255/255.0f};
    color_target_info.texture = scene_texture;
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;

    depth_target_info = {};
    depth_target_info.clear_depth = 1.0f;
    depth_target_info.texture = depth_texture;
    depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    depth_target_info.store_op = SDL_GPU_STOREOP_STORE;

    render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, &depth_target_info);
}

void E64::Renderer::beginUIRenderPass(){
    ImGui::Render();
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buf, E64::Window::getWindow(), &swapchain, &width, &height);

    ImGui_ImplSDLGPU3_PrepareDrawData(ImGui::GetDrawData(), cmd_buf);

    color_target_info = {};
    color_target_info.clear_color = {0/255.0f, 0/255.0f, 0/255.0f, 255/255.0f};
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;
    color_target_info.texture = swapchain;

    render_pass = SDL_BeginGPURenderPass(cmd_buf, &color_target_info, 1, nullptr);
}

void E64::Renderer::endRenderPass(){
    SDL_EndGPURenderPass(render_pass);
}

void E64::Renderer::bindPipeline(SDL_GPUGraphicsPipeline* pipeline){
    SDL_BindGPUGraphicsPipeline(render_pass, pipeline);
}

void E64::Renderer::bindVertexBuffers(E64::ECS::Mesh* mesh){
    SDL_BindGPUVertexBuffers(render_pass, 0, mesh->vbo->getBufferBinding(), 1);
}

void E64::Renderer::bindIndexBuffers(E64::ECS::Mesh* mesh){
    SDL_GPUBuffer* index_buffer = mesh->ibo->getIndexBuffer();
    SDL_GPUBufferBinding binding;
    binding.buffer = index_buffer;
    binding.offset = 0;

    SDL_BindGPUIndexBuffer(render_pass, &binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
}

void E64::Renderer::draw(E64::ECS::Mesh* mesh){
    SDL_DrawGPUIndexedPrimitives(render_pass, mesh->ibo->getNumIndices(), 1, 0, 0, 0);
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

SDL_GPUTexture* E64::Renderer::getDepthTexture(){
    return depth_texture;
}

SDL_GPUTexture* E64::Renderer::getSceneTexture(){
    return scene_texture;
}