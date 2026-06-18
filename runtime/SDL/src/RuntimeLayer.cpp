#include "RuntimeLayer.h"

using namespace E64;

Runtime::RuntimeLayer::RuntimeLayer(){
    angle = 0.0f;
}

Runtime::RuntimeLayer::~RuntimeLayer(){
    
}

void Runtime::RuntimeLayer::OnUpdate(float dt){
    Scene* scene = E64::Engine::ctx->active_scene.get();
    for(ECS::Entity e : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::PointLightComponent>(e) && ECS::ComponentManager::hasComponent<ECS::TransformComponent>(e)){
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);

            if(angle >= 2 * M_PI) angle = 0.0f;
            transform->position.x = 2.0f * std::cos(angle);
            transform->position.z = 2.0f * std::sin(angle);
            angle += 0.002f;
        }
    }
}