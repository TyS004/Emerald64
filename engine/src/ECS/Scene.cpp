#include "ECS/Scene.h"
#include "ECS/ComponetManager.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"

GCGameEngine::Scene::Scene(){
    this->camera = new Camera();
}

GCGameEngine::Scene::~Scene(){
    GCGameEngine::ECS::ComponetManager::clean();
    delete this->camera;
}

void GCGameEngine::Scene::pushEntity(ECS::Entity* entity){
    entites.push_back(*entity);
    return;
}

std::vector<GCGameEngine::ECS::Entity> GCGameEngine::Scene::getEntites(){
    return this->entites;
}

void GCGameEngine::Scene::OnUpdate(){

}

GCGameEngine::Camera* GCGameEngine::Scene::getCamera(){
    return this->camera;
}

void GCGameEngine::Scene::PrintScene(){
    for(GCGameEngine::ECS::Entity e : entites){
        GCGameEngine::Log::info("Entity: " + std::to_string(e));
        if(GCGameEngine::ECS::EntityManager::entity_index[e] & GCGameEngine::ECS::TRANSFORM)
        {
            GCGameEngine::Log::info("Transform");
        }
        else if(GCGameEngine::ECS::EntityManager::entity_index[e] & GCGameEngine::ECS::MESH)
        {
            GCGameEngine::Log::info("Mesh");
        }
    }
}