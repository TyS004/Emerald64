#include "Layer/SceneLayer.h"
#include "Engine.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "AssetManager/AssetManager.h"

#include "Serialization/SceneSerializer.h"
#include "Serialization/MeshSerializer.h"

#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>

E64::SceneLayer::SceneLayer(){
    E64::Log::info("Scene Layer Created\n");

    ECS::ComponentRegistry<ECS::TransformComponent>::registerComponent("Transform");
    ECS::ComponentRegistry<ECS::MeshComponent>::registerComponent("Mesh");
    ECS::ComponentRegistry<ECS::CameraComponent>::registerComponent("Camera");
    ECS::ComponentRegistry<ECS::PointLightComponent>::registerComponent("PointLight");

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
        scene = scene_serializer.deserialize(E64::Engine::ctx->asset_manager->getProjectDir() + "/scenes/scene.json");
        if(!scene){ E64::Log::error("Scene Could Not Be Loaded"); }
        else{ E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(*scene)); }

        E64::Log::info("SCENE: " + scene->getName() + " LOADED");
        scene->printScene();
    }
    scene->printScene();
}

E64::SceneLayer::~SceneLayer(){

}

void E64::SceneLayer::OnRender(){
    scene = E64::Engine::ctx->active_scene.get();
    if(!scene) { E64::Log::error("NO SCENE"); return; }

    E64::IRenderer* renderer = E64::Engine::ctx->renderer;
    renderer->setColorLoadOP(E64::RenderLoadOP::CLEAR);
    renderer->setStencilLoadOP(E64::RenderLoadOP::CLEAR);
    renderer->setDepthLoadOP(E64::RenderLoadOP::CLEAR);
    if (E64::Engine::ctx->mode == EDITOR) {
        renderer->beginRenderPass(RenderTarget::TEXTURE);
    }
    else if (E64::Engine::ctx->mode == DESKTOP_RUNTIME) {
        renderer->beginRenderPass(RenderTarget::SWAPCHAIN);
    }
    renderer->bindPipeline();

    std::vector<ECS::PointLightUniform> light_uniforms{};
    for(ECS::Entity entity : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::PointLightComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity)){
            ECS::PointLightComponent* light_comp = ECS::ComponentManager::getComponent<ECS::PointLightComponent>(entity);
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);

            ECS::PointLightUniform uniform;
            float position[3] = {transform->position.x, transform->position.y, transform->position.z};
            float color[4] = {light_comp->color.r, light_comp->color.g, light_comp->color.b, light_comp->color.a};
            memcpy(uniform.position, position, sizeof(float) * 3);
            memcpy(uniform.color, color, sizeof(float) * 4);
            uniform.intensity = light_comp->intensity;
            light_uniforms.push_back(uniform);
        }
    }
    uint32_t num_pointlights = light_uniforms.size();
    renderer->pushFragmentUniform(light_uniforms.data(), sizeof(ECS::PointLightUniform) * num_pointlights, 0);
    renderer->pushFragmentUniform(&num_pointlights, sizeof(uint32_t), 1);


    for(ECS::Entity entity : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::CameraComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity) &&
            (E64::Engine::ctx->mode == DESKTOP_RUNTIME || E64::Engine::ctx->mode == N64_RUNTIME))
        {
            ECS::CameraComponent* camera = ECS::ComponentManager::getComponent<ECS::CameraComponent>(entity);
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);

            if (camera->active_camera) {
                glm::vec3 direction;
                direction.x = cos(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
                direction.y = sin(glm::radians(transform->euler.y));
                direction.z = sin(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
                glm::vec3 camera_front = glm::normalize(direction);

                glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0, 1, 0)));
                glm::vec3 camera_up = glm::normalize(glm::cross(camera_right, camera_front));

                glm::mat4 view = glm::lookAt(
                    transform->position,                 
                    transform->position + camera_front,  // Looking at 
                    camera_up         
                );

                glm::mat4 proj = glm::perspective(
                    camera->fov,                // FOV
                    camera->aspect_ratio,       // Aspect Ratio
                    camera->near_plane,         // Near plane
                    camera->far_plane           // Far Plane
                );
                scene->setCamera(entity);
                scene->setCameraData({ proj, view });
            }
        }

        if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity))
        {
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);
            ECS::MeshComponent* mesh_componet  = ECS::ComponentManager::getComponent<ECS::MeshComponent>(entity);
            
            Mesh* mesh = E64::Engine::ctx->asset_manager->getMeshAsset(mesh_componet->mesh_handle);
            if(mesh == nullptr) { 
                E64::Log::error("MESH NOT FOUND FOR Component IN ENTITY: " + std::to_string(entity) + "WHEN RENDERING IN SCENELAYER"); 
                mesh = E64::Engine::ctx->asset_manager->getMeshAsset(0); // MESH FOR MISSING MESH
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position)
                * glm::eulerAngleXYZ(transform->euler.x, transform->euler.y, transform->euler.z)
                * glm::scale(glm::mat4(1.0f), transform->scale);
                
            glm::mat4 inv = glm::inverse(model);
            glm::mat4 transpose = glm::transpose(model);
            glm::mat4 normalMat = glm::transpose(glm::inverse(model));

            glm::mat4 view = scene->getCameraData().view;
            glm::mat4 proj = scene->getCameraData().proj;

            renderer->pushVertexUniform(&model, sizeof(glm::mat4), 0);
            renderer->pushVertexUniform(&view, sizeof(glm::mat4), 1);
            renderer->pushVertexUniform(&proj, sizeof(glm::mat4), 2);
            renderer->pushVertexUniform(&normalMat, sizeof(glm::mat4), 3);

            renderer->draw(mesh_componet);
        }
    }
    renderer->endRenderPass();
}