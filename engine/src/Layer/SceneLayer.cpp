#include "Layer/SceneLayer.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "AssetManager/AssetManager.h"

#include <cstdlib>
#include <ctime>
#include <random>

E64::SceneLayer::SceneLayer(){
    scene = E64::Engine::ctx->active_scene;

    AssetManager* asset_manager = new E64::AssetManager();
    E64::Engine::ctx->asset_manager = asset_manager;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-100, 100);

    for(int i = 0; i < 1000; ++i){
        ECS::Entity e1 = E64::ECS::EntityManager::createEntity();
        scene->pushEntity(&e1);
    }

    //Creating Copies for Same Mesh Need to Fix 
    ECS::Mesh obj_mesh = ECS::Mesh{ new VBO(), new IBO()};
    ECS::MeshComponet mesh_comp = { asset_manager->addMesh(obj_mesh) };

    std::vector<E64::ECS::Entity> entities = scene->getEntites();
    for(ECS::Entity e : entities){
        float x = dist(gen);
        float y = dist(gen);
        float z = dist(gen);

        ECS::TransformComponet transform = ECS::TransformComponet{ glm::vec3(x, y, z) };

        ECS::ComponetManager::addComponet<ECS::TransformComponet>(e, transform);
        ECS::ComponetManager::addComponet<ECS::MeshComponet>(e, mesh_comp);
    }

    std::cout << "\tScene: " << scene << std::endl;
}

E64::SceneLayer::~SceneLayer(){
    delete scene;
}

void E64::SceneLayer::OnRender(){
    scene->render();
}