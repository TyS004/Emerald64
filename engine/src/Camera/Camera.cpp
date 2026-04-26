#include "Camera/Camera.h"

GCGameEngine::Camera::Camera(){
    this->transform = { glm::vec3(1.0) };
    
    this->view = glm::lookAt(
        glm::vec3(1, 1, 1),  // Camera Pos
        glm::vec3(0, 0, 0),  // Looking at Origin
        glm::vec3(0, 1, 0)   // Up Vector 
    );

    this->proj = glm::perspective(
        glm::radians(45.0f), // FOV
        800.0f / 600.0f,     // Aspect Ratio
        0.1f,                // Near plane
        100.0f               // Far Plane
    );
}

GCGameEngine::Camera::~Camera(){

}

void GCGameEngine::Camera::OnUpdate(){
    return;
}

GCGameEngine::Componet::Transform GCGameEngine::Camera::getTransform(){
    this->transform;
}

void GCGameEngine::Camera::setTransform(GCGameEngine::Componet::Transform transform){
    this->transform = transform;
}

glm::mat4 GCGameEngine::Camera::getView(){
    return this->view;
}

glm::mat4 GCGameEngine::Camera::getProj(){
    return this->proj;
}