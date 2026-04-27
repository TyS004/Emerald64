#include "Object/Object.h"

GCGameEngine::Object::Object(){
    this->mesh = { new VBO(3) };
    this->transform = { glm::vec3(0.0f, 0.0f, 0.0f) };
    this->model = glm::mat4(1.0f);
    this->mvp = glm::mat4(1.0f);
    
    this->model = glm::translate(glm::mat4(1.0f), transform.transform);
}

GCGameEngine::Object::~Object(){

}

glm::mat4 GCGameEngine::Object::getModel(){
    return this->model;
}

void GCGameEngine::Object::OnUpdate(Camera* camera){
    this->model = glm::translate(glm::mat4(1.0f), transform.transform);
    this->mvp = camera->getProj() * camera->getView() * this->model;
}

void GCGameEngine::Object::setMVP(glm::mat4 mvp){
    this->mvp = mvp;
}

glm::mat4 GCGameEngine::Object::getMVP(){
    return this->mvp;
}