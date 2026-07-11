#include "Layer/PhysicsLayer.h"

#include "ECS/ComponentManager.h"

#include "Engine.h"

E64::PhysicsLayer::PhysicsLayer(){
    velocity = 0.0f;
    acceleration = -9.8f;

    scene = E64::Engine::ctx->active_scene.get();
}

E64::PhysicsLayer::~PhysicsLayer(){

}

void E64::PhysicsLayer::OnUpdate(float dt){
    for(ECS::Entity e : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::RigidbodyComponent>(e))
        {
            ECS::TransformComponent* t = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);
            t->position.y += velocity * dt;
        }
    }
    velocity += acceleration * dt;

    CameraUpdate();
}

void E64::PhysicsLayer::CameraUpdate() {
    E64::Input* input = E64::Engine::ctx->input;
    ECS::Entity camera = scene->getCamera();

    ECS::TransformComponent* camera_transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(camera);
    if (camera_transform) {
        if (input->isKeyPressed(E64::Scancode::W))
        {
            camera_transform->position.z += 0.01f;
        }
        if (input->isKeyPressed(E64::Scancode::A))
        {
            camera_transform->position.x += 0.01f;
        }
        if (input->isKeyPressed(E64::Scancode::S))
        {
            camera_transform->position.z -= 0.01f;
        }
        if (input->isKeyPressed(E64::Scancode::D))
        {
            camera_transform->position.x -= 0.01f;
        }
    }
}