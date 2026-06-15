#include "Layer/PhysicsLayer.h"

#include "ECS/ComponentManager.h"

#include "Engine.h"

E64::PhysicsLayer::PhysicsLayer(){
    velocity = 0.0f;
    acceleration = -.01f;
}

E64::PhysicsLayer::~PhysicsLayer(){

}

void E64::PhysicsLayer::OnUpdate(float dt){
    E64::Scene* scene = E64::Engine::ctx->active_scene.get();
    for(ECS::Entity e : scene->getEntites()){
        if(!ECS::ComponentManager::hasComponent<ECS::CameraComponent>(e))
        {
            ECS::TransformComponent* t = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);
            t->position.y += velocity * dt;
        }
    }
    velocity += acceleration;
}