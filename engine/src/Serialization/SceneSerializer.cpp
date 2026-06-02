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

E64::Scene* E64::SceneSerializer::deserialize(){
    ECS::EntityManager::flushEntites();
    ECS::ComponentManager::flushComponents();

    E64::Scene* scene = new Scene();

    std::ifstream file("../assets/scenes/scene.json");
    if(!file){
        E64::Log::debug("CANNOT OPEN FILE");
        return nullptr;
    }
    json scene_json;
    file >> scene_json;
    file.close();

    for (const json& entity_json : scene_json["entities"]) {
        ECS::Entity e = entity_json["id"];
        scene->pushEntity(e);
        ECS::ComponentManager::deserialize(entity_json, e);
    }

    E64::Log::debug("Loaded Scene: " + scene->getName());
    scene->printScene();

    return scene;
}