#include "ECS/Scene.h"
#include "ECS/ComponetManager.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"
#include "Engine.h"

E64::Scene::Scene(){

}

E64::Scene::~Scene(){

}

void E64::Scene::pushEntity(ECS::Entity* entity){
    entites.push_back(*entity);
    return;
}

std::vector<E64::ECS::Entity> E64::Scene::getEntites(){
    return this->entites;
}

void E64::Scene::printScene(){
    for(E64::ECS::Entity e : entites){
        std::string msg = "{ ";
        if(E64::ECS::EntityManager::entity_index[e] & E64::ECS::TRANSFORM)
        {
            msg += "Transform ";
        }
        if(E64::ECS::EntityManager::entity_index[e] & E64::ECS::MESH)
        {
            msg += "Mesh ";
        }
        msg += "}";
        E64::Log::info("Entity " + std::to_string(e) + ": " + msg);
    }
}

void E64::Scene::setCameraData(E64::CameraData camera_data){
    this->camera_data = camera_data;
}

void E64::Scene::render(){
    E64::Renderer* renderer = E64::Engine::ctx->renderer;

    for(ECS::Entity entity : entites){
        if(ECS::ComponetManager::hasComponet<ECS::MeshComponet>(entity) &&
            ECS::ComponetManager::hasComponet<ECS::TransformComponet>(entity))
        {
            ECS::TransformComponet* transform = ECS::ComponetManager::getComponet<ECS::TransformComponet>(entity);
            ECS::MeshComponet* mesh_componet = ECS::ComponetManager::getComponet<ECS::MeshComponet>(entity);
           
            ECS::Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_componet->id);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position);
            glm::mat4 mvp = camera_data.proj * camera_data.view * model;

            renderer->bindVertexBuffers(mesh);
            renderer->bindIndexBuffers(mesh);
            renderer->sendUniforms(mvp);
            renderer->draw(mesh);
        }
    }
}