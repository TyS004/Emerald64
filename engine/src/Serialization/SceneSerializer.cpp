#include "Serialization/SceneSerializer.h"

using json = nlohmann::json;

E64::SceneSerializer::SceneSerializer(){
    
}

E64::SceneSerializer::~SceneSerializer(){
    
}

void E64::SceneSerializer::serialize(){
    std::ofstream file("../assets/scenes/scene.json");
    json scene_json;
    E64::Scene* scene = E64::Engine::ctx->active_scene.get();
    
    scene_json["name"] = scene->getName();
    scene_json["entites"] = json::array();

    for(ECS::Entity e : scene->getEntites()){
        json entity_json;
        entity_json["id"] = e;
            
        if(ECS::ComponetManager::hasComponet<ECS::TransformComponet>(e)){
            json transform_json;
            ECS::TransformComponet* comp = ECS::ComponetManager::getComponet<ECS::TransformComponet>(e);
            comp->serialize(transform_json);

            entity_json["Transform"] = transform_json;
        }
        if(ECS::ComponetManager::hasComponet<ECS::MeshComponet>(e)){
            json mesh_json;
            ECS::MeshComponet* comp = ECS::ComponetManager::getComponet<ECS::MeshComponet>(e);
            comp->serialize(mesh_json);

            entity_json["Mesh"] = mesh_json;
        }
        
        scene_json["entites"].push_back(entity_json);
    }

    file << scene_json.dump(4);
}

void E64::SceneSerializer::deserialize(){
    ECS::EntityManager::flushEntites();
    ECS::ComponetManager::flushComponets();

    E64::Scene scene;
    scene.setCameraData(E64::Engine::ctx->active_scene->getCameraData());

    std::ifstream file("../assets/scenes/scene.json");
    json scene_json;
    file >> scene_json;

    for (const json& entity_json : scene_json["entites"]) {
        ECS::Entity e = entity_json["id"];
        scene.pushEntity(e);
        ECS::EntityManager::createEntity(e);
        if(entity_json.contains("Transform")){
            json transform_json = entity_json["Transform"];
            ECS::TransformComponet transform;
            transform.deserialize(transform_json);
            ECS::ComponetManager::addComponet<ECS::TransformComponet>(e, transform);
        }
        if(entity_json.contains("Mesh")){
            json mesh_json = entity_json["Mesh"];
            ECS::MeshComponet mesh;
            mesh.deserialize(mesh_json);
            ECS::ComponetManager::addComponet<ECS::MeshComponet>(e, mesh);
        }
    }

    scene.printScene();
    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(scene));
}