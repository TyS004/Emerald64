#include "Engine.h"

bool GCGameEngine::Engine::running = true;
std::vector<GCGameEngine::Layer*> GCGameEngine::Engine::layers = {};
GCGameEngine::Scene* GCGameEngine::Engine::active_scene = nullptr;

void GCGameEngine::Engine::run(){
    SDL_GPUDevice* device = GCGameEngine::Window::getDevice();
    SDL_Window* window = GCGameEngine::Window::getWindow();
    
    Pipeline* pipeline = new Pipeline("../assets/shaders/object");

    if (!active_scene) {
        std::cout << "No active scene!" << std::endl;
        return;
    }

    std::cout << "Scene: " << active_scene << std::endl;
    for(GCGameEngine::ECS::Entity e : active_scene->getEntites()){
        
        std::cout << "Entity ID: " << e << std::endl;
    }

    SDL_GPUTextureCreateInfo depth_texture_info = {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
        .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
        .width = GCGameEngine::Window::getWidth(),
        .height = GCGameEngine::Window::getHeight(),
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1
    };
    SDL_GPUTexture* depth_texture = SDL_CreateGPUTexture(device, &depth_texture_info);

    while(running){
        GCGameEngine::Window::PollEvent();
        if(!GCGameEngine::Input::isRunning()) break;
        
        for(Layer* layer : layers){
            layer->OnUpdate();
        }

        SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);

        SDL_GPUTexture* swapchain;
        Uint32 width, height;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmd_buffer, window, &swapchain, &width, &height);

        SDL_GPUColorTargetInfo color_target_info{};
        color_target_info.clear_color = {80/255.0f, 80/255.0f, 80/255.0f, 255/255.0f};
        color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        color_target_info.store_op = SDL_GPU_STOREOP_STORE;
        color_target_info.texture = swapchain;

        SDL_GPUDepthStencilTargetInfo depth_target_info{};
        depth_target_info.texture = depth_texture;
        depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        depth_target_info.store_op = SDL_GPU_STOREOP_STORE;
        depth_target_info.clear_depth = 1.0f;

        GCGameEngine::Renderer::begin(cmd_buffer, &color_target_info, &depth_target_info);
        GCGameEngine::Renderer::bindPipeline(pipeline->getPipeline());

        for(ECS::Entity entity : active_scene->getEntites()){
            if(ECS::ComponetManager::hasComponet<ECS::Mesh>(entity)){
                ECS::Transform transform = *ECS::ComponetManager::getComponet<ECS::Transform*>(entity);
                ECS::Mesh mesh = *ECS::ComponetManager::getComponet<ECS::Mesh*>(entity);
    
                glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
                glm::mat4 mvp = active_scene->getCamera()->getProj() * active_scene->getCamera()->getView() * model;
    
                GCGameEngine::Renderer::bindVertexBuffers(mesh.vbo->getBufferBinding());
                GCGameEngine::Renderer::sendUniforms(cmd_buffer, mvp);
                GCGameEngine::Renderer::draw();
            }
        }

        GCGameEngine::Renderer::end();
        SDL_SubmitGPUCommandBuffer(cmd_buffer);
    }
    GCGameEngine::Input::clean();
}

void GCGameEngine::Engine::exit(){
    running = false;
}

void GCGameEngine::Engine::pushLayer(Layer* layer){
    GCGameEngine::Engine::layers.push_back(layer);
}

void GCGameEngine::Engine::setActiveScene(Scene* scene){
    GCGameEngine::Engine::active_scene = scene;
}