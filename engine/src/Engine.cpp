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

        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.clear_color = {80/255.0f, 80/255.0f, 80/255.0f, 255/255.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.texture = swapchain;

        GCGameEngine::Renderer::begin(cmd_buffer, &colorTargetInfo);
        GCGameEngine::Renderer::bindPipeline(pipeline->getPipeline());

        for(ECS::Entity entity : active_scene->getEntites()){
            if(ECS::ComponetManager::hasComponet<ECS::Mesh>(entity)){
                ECS::Transform transform = *ECS::ComponetManager::getComponet<ECS::Transform*>(0);
                ECS::Mesh mesh = *ECS::ComponetManager::getComponet<ECS::Mesh*>(entity);
    
                glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.transform);
                glm::mat4 mvp = active_scene->getCamera()->getProj() * active_scene->getCamera()->getView() * model;
    
                GCGameEngine::Renderer::bindVertexBuffers(mesh.vbo->getBufferBinding());
                GCGameEngine::Renderer::sendUniforms(cmd_buffer, mvp);
                GCGameEngine::Renderer::draw();
            }
        }

        GCGameEngine::Renderer::end();
        SDL_SubmitGPUCommandBuffer(cmd_buffer);
    }
}

void GCGameEngine::Engine::pushLayer(Layer* layer){
    GCGameEngine::Engine::layers.push_back(layer);
}

void GCGameEngine::Engine::setActiveScene(Scene* scene){
    GCGameEngine::Engine::active_scene = scene;
}