#include "Layer/SceneLayer.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "AssetManager/AssetManager.h"
#include "Serialization/SceneSerializer.h"

#include <cstdlib>
#include <ctime>
#include <random>

E64::SceneLayer::SceneLayer(){
    ECS::ComponentRegistry<ECS::TransformComponent>::registerComponent("Transform");
    ECS::ComponentRegistry<ECS::MeshComponent>::registerComponent("Mesh");
    ECS::ComponentRegistry<ECS::CameraComponent>::registerComponent("Camera");

    E64::Engine::ctx->asset_manager = std::make_unique<E64::AssetManager>();

    scene = E64::Engine::ctx->active_scene.get();
    if(scene == nullptr){
        SceneSerializer serializer;
        serializer.deserialize();
        scene = E64::Engine::ctx->active_scene.get();
    }
    else{
        std::cout << "RANDOM SCENE" << std::endl;
        scene->createDefaultScene();
    }

    std::unique_ptr<Pipeline> pipeline = std::make_unique<Pipeline>("../assets/shaders/object");

    std::cout << "\tScene: " << scene << std::endl;
    scene->printScene();
}

E64::SceneLayer::~SceneLayer(){

}

void E64::SceneLayer::OnRender(){
    scene = E64::Engine::ctx->active_scene.get();
    if(!scene) { E64::Log::error("NO SCENE"); return; }

    E64::Renderer* renderer = E64::Engine::ctx->renderer.get();

    if(E64::Engine::ctx->editor_mode)
    {
        renderer->beginRenderPass(RenderTarget::TEXTURE);
    }
    else{
        renderer->beginRenderPass(RenderTarget::SWAPCHAIN);
    }
    renderer->bindPipeline();
    scene->render();
    renderer->endRenderPass();
}