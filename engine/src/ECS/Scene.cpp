#include "ECS/Scene.h"
#include "ECS/ComponentManager.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"
#include "Engine.h"

#include <cstdlib>
#include <ctime>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

E64::Scene::Scene(){
    name = "Test_Scene";

    ECS::TransformComponent transform = E64::ECS::TransformComponent{};
    transform.position = glm::vec3(0.0, 0.0, -5.0);
    transform.euler = glm::vec3(90.0f, 0.0f, 0.0f);
    
    glm::mat4 view = glm::lookAt(
        transform.position,  // Camera Pos
        glm::vec3(0, 0, 0),         // Looking at Origin
        glm::vec3(0, 1, 0)          // Up Vector 
    );
    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), // FOV
        800.0f / 600.0f,     // Aspect Ratio
        0.1f,                // Near plane
        100.0f               // Far Plane
    );
    active_camera_data = ECS::CameraData{proj, view};

    entites = {};
}

E64::Scene::Scene(Scene&& other) noexcept
    : entites(std::move(other.entites)),
      name(std::move(other.name))
{
    
}

void E64::Scene::createDefaultScene(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-10, 10);

    for(int i = 0; i < 10; ++i){
        pushEntity();
    }

    ECS::MeshComponent mesh_comp = {{0, "default"}};
    for(ECS::Entity e : entites){
        float x = dist(gen);
        float y = dist(gen);
        float z = dist(gen);

        ECS::TransformComponent transform = ECS::TransformComponent{ glm::vec3(x, y, z) };

        ECS::ComponentManager::addComponent<ECS::TransformComponent>(e, transform);
        ECS::ComponentManager::addComponent<ECS::MeshComponent>(e, mesh_comp);
    }

    ECS::Entity camera_entity = ECS::EntityManager::createEntity();
    entites.push_back(camera_entity);

    ECS::CameraComponent camera = {
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,               
        100.0f
    };
    ECS::TransformComponent transform {};
    transform.position.z = -30.0f;
    transform.euler.x = 90.0f;
    
    ECS::ComponentManager::addComponent<ECS::CameraComponent>(camera_entity, camera);
    ECS::ComponentManager::addComponent<ECS::TransformComponent>(camera_entity, transform);
}

E64::Scene::~Scene(){

}

std::string E64::Scene::getName(){
    return name;
}

E64::ECS::CameraData E64::Scene::getCameraData(){
    return active_camera_data;
}

void E64::Scene::pushEntity(){
    ECS::Entity e = ECS::EntityManager::createEntity();
    entites.push_back(e);
    return;
}

void E64::Scene::pushEntity(ECS::Entity e){
    ECS::EntityManager::registerEntity(e);
    entites.push_back(e);
    return;
}

std::vector<E64::ECS::Entity> E64::Scene::getEntites(){
    return this->entites;
}

void E64::Scene::printScene(){
    for(E64::ECS::Entity e : entites){
        std::string msg = "{ ";
        if(E64::ECS::EntityManager::entity_index[e] & E64::ECS::ComponentBit<ECS::TransformComponent>::mask)
        {
            msg += "Transform ";
        }
        if(E64::ECS::EntityManager::entity_index[e] & E64::ECS::ComponentBit<ECS::MeshComponent>::mask)
        {
            msg += "Mesh ";
        }
        if(E64::ECS::EntityManager::entity_index[e] & E64::ECS::ComponentBit<ECS::CameraComponent>::mask)
        {
            msg += "Camera ";
        }
        msg += "}";
        E64::Log::info("Entity " + std::to_string(e) + ": " + msg);
    }
}

void E64::Scene::setCameraData(E64::ECS::CameraData camera_data){
    this->active_camera_data = camera_data;
}

void E64::Scene::render(){
    E64::IRenderer* renderer = E64::Engine::ctx->renderer;

    for(ECS::Entity entity : entites){
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
            active_camera_data = {proj, view};
        }

        if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(entity) &&
            ECS::ComponentManager::hasComponent<ECS::TransformComponent>(entity))
        {
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(entity);
            ECS::MeshComponent* mesh_componet  = ECS::ComponentManager::getComponent<ECS::MeshComponent>(entity);
            
            ECS::Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_componet->mesh_handle);
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

            glm::mat4 mvp = active_camera_data.proj * active_camera_data.view * model;

            renderer->bindVertexBuffers(mesh);
            renderer->bindIndexBuffers(mesh);
            renderer->bindFragmentSamplers(mesh);
            renderer->sendUniforms(mvp);
            renderer->draw(mesh);
        }
    }
}