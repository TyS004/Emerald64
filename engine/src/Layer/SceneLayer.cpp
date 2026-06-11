#include "Layer/SceneLayer.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "AssetManager/AssetManager.h"

#include "Serialization/SceneSerializer.h"
#include "Serialization/MeshSerializer.h"

#include <cstdlib>
#include <ctime>
#include <random>

E64::SceneLayer::SceneLayer(){
    E64::Log::info("Scene Layer Created\n");

    ECS::ComponentRegistry<ECS::TransformComponent>::registerComponent("Transform");
    ECS::ComponentRegistry<ECS::MeshComponent>::registerComponent("Mesh");
    ECS::ComponentRegistry<ECS::CameraComponent>::registerComponent("Camera");

    E64::Engine::ctx->asset_manager = std::make_unique<E64::AssetManager>();
    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>();

    if(E64::Engine::ctx->mode == EDITOR || E64::Engine::ctx->mode == N64_RUNTIME){
        E64::Log::info("Creating Default Scene");
        scene = E64::Engine::ctx->active_scene.get();
        scene->createDefaultScene();
    }
    else if(E64::Engine::ctx->mode == DESKTOP_RUNTIME){
        //Deserialize at Runtime
        E64::Log::info("Loading Scene");
        //Scene
        SceneSerializer scene_serializer;
        scene = scene_serializer.deserialize();
        if(!scene){ E64::Log::error("Scene Could Not Be Loaded"); }
        else{ E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(*scene)); }

        //Meshes
        MeshSerializer  mesh_serializer;
        for (const auto& entry : std::filesystem::directory_iterator("../assets/meshes/")) {
            if(entry.path().extension() == ".e64mesh")
            {
                Mesh mesh = mesh_serializer.deserialize(entry.path().string());
                E64::Engine::ctx->asset_manager->addMesh(mesh);
            }
        }
        E64::Log::info("SCENE: " + scene->getName() + " LOADED");
    }
    scene->printScene();
}

E64::SceneLayer::~SceneLayer(){

}

void E64::SceneLayer::OnRender(){
    scene = E64::Engine::ctx->active_scene.get();
    if(!scene) { E64::Log::error("NO SCENE"); return; }

    E64::IRenderer* renderer = E64::Engine::ctx->renderer;

    if(E64::Engine::ctx->mode == EDITOR){
        renderer->beginRenderPass(RenderTarget::TEXTURE);
    }
    else{
        renderer->beginRenderPass(RenderTarget::SWAPCHAIN);
    }
    renderer->bindPipeline();
    
    for(ECS::Entity entity : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::CameraComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity) &&
            (E64::Engine::ctx->mode == DESKTOP_RUNTIME || E64::Engine::ctx->mode == N64_RUNTIME))
        {
            ECS::CameraComponent* camera = ECS::ComponentManager::getComponent<ECS::CameraComponent>(entity);
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);

            glm::mat4 view = glm::lookAt(
                transform->position,        // Camera Pos
                glm::vec3(0, 0, 0),         // Looking at Origin
                glm::vec3(0, 1, 0)          // Up Vector 
            );
        
            glm::mat4 proj = glm::perspective(
                camera->fov,                // FOV
                camera->aspect_ratio,       // Aspect Ratio
                camera->near_plane,         // Near plane
                camera->far_plane           // Far Plane
            );
            scene->setCameraData({proj, view});
        }

        if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity))
        {
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);
            ECS::MeshComponent* mesh_componet  = ECS::ComponentManager::getComponent<ECS::MeshComponent>(entity);
            
            Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_componet->mesh_handle);
            if(mesh == nullptr) { 
                E64::Log::error("MESH NOT FOUND FOR Component IN ENTITY: " + std::to_string(entity)); 

                AssetHandle mesh_handle;
                mesh_handle.id = 0;
                mesh_handle.path = "default";
                mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_handle); // MESH FOR MISSING MESH
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position)
                * glm::eulerAngleXYZ(transform->euler.x, transform->euler.y, transform->euler.z)
                * glm::scale(glm::mat4(1.0f), transform->scale);

            glm::mat4 mvp = scene->getCameraData().proj * scene->getCameraData().view * model;

            renderer->sendUniforms(mvp);
            renderer->draw(mesh);
        }
    }

    renderer->endRenderPass();
}