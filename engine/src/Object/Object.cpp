#include "Object/Object.h"

GCGameEngine::Object::Object(){
    mesh = { new VBO(3) };
    transform = { glm::vec3(0.0f, 0.0f, 0.0f) };
    this->model = glm::mat4(1.0f);
    
    model = glm::translate(glm::mat4(1.0f), transform.transform);
}

GCGameEngine::Object::~Object(){

}

GCGameEngine::Componet::Mesh GCGameEngine::Object::getMesh(){
    return this->mesh;
}

glm::mat4 GCGameEngine::Object::getModel(){
    return this->model;
}