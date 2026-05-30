#include "Layer/PhysicsLayer.h"

#include "ECS/ComponentManager.h"

#include "Engine.h"

bool E64::PhysicsLayer::running = false;

E64::PhysicsLayer::PhysicsLayer(){
    velocity = 0.0f;
    acceleration = -.01f;
}

E64::PhysicsLayer::~PhysicsLayer(){

}

void E64::PhysicsLayer::OnUpdate(float dt){
    E64::Scene* scene = E64::Engine::ctx->active_scene.get();
    if(running){
        for(ECS::Entity e : scene->getEntites()){
            ECS::TransformComponent* t = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);
            t->position.y += velocity * dt;
        }
        velocity += acceleration;
    }
}