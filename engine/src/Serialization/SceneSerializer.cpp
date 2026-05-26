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
    scene_json["entities"] = json::array();

    for(ECS::Entity e : scene->getEntites()){
        json entity_json;
        entity_json["id"] = e;
            
        ECS::ComponentManager::serialize(entity_json, e);

        scene_json["entities"].push_back(entity_json);
    }

    file << scene_json.dump(4);
    file.close();
}

void E64::SceneSerializer::deserialize(){
    ECS::EntityManager::flushEntites();
    ECS::ComponentManager::flushComponents();

    E64::Scene scene;

    std::ifstream file("../assets/scenes/scene.json");
    json scene_json;
    file >> scene_json;
    file.close();

    for (const json& entity_json : scene_json["entities"]) {
        ECS::Entity e = entity_json["id"];
        scene.pushEntity(e);
        ECS::ComponentManager::deserialize(entity_json, e);
    }

    E64::Log::debug("Loaded Scene: " + scene.getName());
    scene.printScene();

    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(scene));
}