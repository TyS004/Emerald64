#include "Layer/EditorLayer.h"

using namespace GCGameEngine;
using namespace ECS;

Editor::EditorLayer::EditorLayer(){
    scene = new GCGameEngine::Scene();
    GCGameEngine::Engine::setActiveScene(scene);

    std::vector<GCGameEngine::ECS::Entity> entities = scene->getEntites();
    
    GCGameEngine::ECS::ComponetManager::addComponet<Transform*>(entities[0]);
    GCGameEngine::ECS::ComponetManager::addComponet<Mesh*>(entities[0]);
}

Editor::EditorLayer::~EditorLayer(){
    GCGameEngine::ECS::ComponetManager::clean();
}

void Editor::EditorLayer::OnUpdate(){
    GCGameEngine::ECS::Transform* transform = ComponetManager::getComponet<Transform*>(scene->getEntites().at(0));

    if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_A)){
        GCGameEngine::Log::debug("A Pressed!");
        transform->transform += glm::vec3(-0.05f, 0.0f, 0.0f);
    }
    else if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_D)){
        GCGameEngine::Log::debug("D Pressed!");
        transform->transform += glm::vec3(0.05f, 0.0f, 0.0f);
    }
    else if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_W)){
        GCGameEngine::Log::debug("W Pressed!");
        transform->transform += glm::vec3(0.0f, 0.0f, -0.05f);
    }
    else if(GCGameEngine::Input::isKeyDown(SDL_SCANCODE_S)){
        GCGameEngine::Log::debug("S Pressed!");
        transform->transform += glm::vec3(0.0f, 0.0f, 0.05f);
    }
}