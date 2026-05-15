#include "EditorCamera/EditorCamera.h"
#include "Input/Input.h"
#include "Engine.h"

Editor::EditorCamera::EditorCamera(){
    cam_sens = 0.25f;
    cam_vel = 15.0f;

    this->transform = new E64::ECS::TransformComponet{};
    transform->position = glm::vec3(0.0, 0.0, -5.0);
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

    prev_xrel = prev_yrel = 0.0f;
    xrel = yrel = 0.0f;
}

Editor::EditorCamera::~EditorCamera(){
    delete this->transform;
}

void Editor::EditorCamera::OnUpdate(float dt){
   if(E64::Window::isMouseLock()){
        glm::vec3 direction;
        direction.x = cos(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
        direction.y = sin(glm::radians(transform->euler.y));
        direction.z = sin(glm::radians(transform->euler.x)) * cos(glm::radians(transform->euler.y));
        camera_front = glm::normalize(direction);

        glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0, 1, 0)));
        glm::vec3 camera_up    = glm::normalize(glm::cross(camera_right, camera_front));

        if(E64::Input::isKeyDown(SDL_SCANCODE_W)) { transform->position += (cam_vel * camera_front) * dt; }
        if(E64::Input::isKeyDown(SDL_SCANCODE_S)) { transform->position -= (cam_vel * camera_front) * dt; }
        if(E64::Input::isKeyDown(SDL_SCANCODE_A)) { transform->position -= (cam_vel * glm::normalize(glm::cross(camera_front, camera_up))) * dt; }
        if(E64::Input::isKeyDown(SDL_SCANCODE_D)) { transform->position += (cam_vel * glm::normalize(glm::cross(camera_front, camera_up))) * dt; }

        xrel = E64::Input::getXRel();
        yrel = E64::Input::getYRel();
        if(((prev_xrel != xrel) || (prev_yrel != yrel)))
        {
            transform->euler.x += (xrel * cam_sens);
            transform->euler.y -= (yrel * cam_sens);
            
            transform->euler.y = glm::clamp(transform->euler.y, -89.0f, 89.0f);
        }
        prev_xrel = xrel;
        prev_yrel = yrel;

        this->view = glm::lookAt(
            transform->position,
            transform->position + camera_front,
            camera_up
        );
    }
    E64::Engine::ctx->active_scene->setCameraData({proj, view});
}

void Editor::EditorCamera::OnResize(float width, float height){
    this->proj = glm::perspective(
        glm::radians(45.0f), // FOV
        width / height,      // Aspect Ratio
        0.1f,                // Near plane
        100.0f               // Far Plane
    );
}

E64::ECS::TransformComponet* Editor::EditorCamera::getTransform(){
    return this->transform;
}

void Editor::EditorCamera::setTransform(E64::ECS::TransformComponet* transform){
    this->transform = transform;
}

glm::mat4 Editor::EditorCamera::getView(){
    return this->view;
}

glm::mat4 Editor::EditorCamera::getProj(){
    return this->proj;
}

glm::vec3 Editor::EditorCamera::getCameraFront(){
    return this->camera_front;
}

float Editor::EditorCamera::getSens(){
    return cam_sens;
}

float Editor::EditorCamera::getVelocity(){
    return cam_vel;
}