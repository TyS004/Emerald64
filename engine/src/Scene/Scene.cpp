#include "Scene/Scene.h"

GCGameEngine::Scene::Scene(){
    this->entites = new Entity*[2];

    this->obj = new Object();
    GCGameEngine::Camera* camera = new Camera();

    mvp = camera->getProj() * camera->getView() * obj->getModel();
}

GCGameEngine::Scene::~Scene(){
    delete[] entites;
}

void GCGameEngine::Scene::pushEntity(Entity* entity){
    return;
}

glm::mat4 GCGameEngine::Scene::getMVP(){
    return this->mvp;
}

GCGameEngine::Object* GCGameEngine::Scene::getObject(){
    return this->obj;
}