#include "Camera/Camera.h"
#include "Input/Input.h"

GCGameEngine::Camera::Camera(){
    cam_sens = 0.25f;
    cam_velocity = 0.05f;

    this->transform = new ECS::Transform{};
    transform->position = glm::vec3(0.0, 0.0, -2.0);
    transform->euler = glm::vec3(90.0f, 0.0f, 0.0f);
    
    this->view = glm::lookAt(
        this->transform->position,  // Camera Pos
        glm::vec3(0, 0, 0),         // Looking at Origin
        glm::vec3(0, 1, 0)          // Up Vector 
    );

    this->proj = glm::perspective(
        glm::radians(45.0f), // FOV
        800.0f / 600.0f,     // Aspect Ratio
        0.1f,                // Near plane
        100.0f               // Far Plane
    );
}

GCGameEngine::Camera::~Camera(){
    delete this->transform;
}

void GCGameEngine::Camera::OnUpdate(){
    std::cout << "Camera Transform: " <<
       this->transform->position.x << " " << this->transform->position.y << " " << this->transform->position.z << " " << std::endl;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
    direction.y = sin(glm::radians(transform->euler.y));
    direction.z = sin(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
    camera_front = glm::normalize(direction);

    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0, 1, 0)));
    glm::vec3 camera_up    = glm::normalize(glm::cross(camera_right, camera_front));

    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_W)) { transform->position += 0.05f * camera_front; }
    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_S)) { transform->position -= 0.05f * camera_front; }
    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_A)) { transform->position -= 0.05f * glm::normalize(glm::cross(camera_front, camera_up)); }
    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_D)) { transform->position += 0.05f * glm::normalize(glm::cross(camera_front, camera_up)); }

    this->view = glm::lookAt(
        transform->position,
        transform->position + camera_front,
        camera_up
    );
}

GCGameEngine::ECS::Transform* GCGameEngine::Camera::getTransform(){
    return this->transform;
}

void GCGameEngine::Camera::setTransform(GCGameEngine::ECS::Transform* transform){
    this->transform = transform;
}

glm::mat4 GCGameEngine::Camera::getView(){
    return this->view;
}

glm::mat4 GCGameEngine::Camera::getProj(){
    return this->proj;
}

glm::vec3 GCGameEngine::Camera::getCameraFront(){
    return this->camera_front;
}

float GCGameEngine::Camera::getSens(){
    return cam_sens;
}

float GCGameEngine::Camera::getVelocity(){
    return cam_velocity;
}