#include "Layer/SceneLayer.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>

E64::SceneLayer::SceneLayer(){
    scene = E64::Engine::ctx->active_scene;
    FBXParser* parser = new FBXParser("../assets/meshes/gemstone.obj");

    ECS::Entity e1 = E64::ECS::EntityManager::createEntity();
    scene->pushEntity(&e1);

    std::vector<E64::ECS::Entity> entities = scene->getEntites();
    
    ECS::Transform* transform = new ECS::Transform{};
    ECS::Mesh* obj_mesh = parser->getMesh();
    ECS::ComponetManager::addComponet<ECS::Transform*>(entities[0], transform);
    ECS::ComponetManager::addComponet<ECS::Mesh*>(entities[0], obj_mesh);

    std::cout << "Scene: " << scene << std::endl;
    scene->printScene();

    delete parser;
}

E64::SceneLayer::~SceneLayer(){
    delete scene;
}

void E64::SceneLayer::OnRender(){
    scene->render();
}