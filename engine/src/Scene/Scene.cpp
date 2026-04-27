#include "Scene/Scene.h"
#include "ECS/ComponetManager.h"
#include "Renderer/Renderer.h"
#include "ECS/Entity.h"

GCGameEngine::Scene::Scene(){
    GCGameEngine::ECS::Entity e = GCGameEngine::ECS::EntityManager::createEntity();
    entites.push_back(e);
    this->camera = new Camera();
}

GCGameEngine::Scene::~Scene(){
    delete this->camera;
}

void GCGameEngine::Scene::pushEntity(ECS::Entity* entity){
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