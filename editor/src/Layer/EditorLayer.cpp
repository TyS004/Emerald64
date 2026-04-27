#include "Layer/EditorLayer.h"
#include "EditorInput/EditorInput.h"

using namespace GCGameEngine;
using namespace ECS;

GCGameEngine::Scene* Editor::EditorLayer::scene = new GCGameEngine::Scene();

Editor::EditorLayer::EditorLayer(){
    Entity e1 = GCGameEngine::ECS::EntityManager::createEntity();
    Entity e2 = GCGameEngine::ECS::EntityManager::createEntity();

    scene->pushEntity(&e1);
    scene->pushEntity(&e2);

    GCGameEngine::Engine::setActiveScene(scene);

    std::vector<GCGameEngine::ECS::Entity> entities = scene->getEntites();
    
    GCGameEngine::ECS::ComponetManager::addComponet<Transform*>(entities[0]);
    GCGameEngine::ECS::ComponetManager::addComponet<Transform*>(entities[1]);
    GCGameEngine::ECS::ComponetManager::addComponet<Mesh*>(entities[0]);
    GCGameEngine::ECS::ComponetManager::addComponet<Mesh*>(entities[1]);

    GCGameEngine::Input::OnKeyPressedBind = Editor::EditorInput::OnKeyPressed;
    GCGameEngine::Input::OnKeyDownBind = Editor::EditorInput::OnKeyDown;
}

Editor::EditorLayer::~EditorLayer(){
    delete scene;
    GCGameEngine::ECS::ComponetManager::clean();
}

void Editor::EditorLayer::OnUpdate(){
    GCGameEngine::ECS::Transform* transform1 = ECS::ComponetManager::getComponet<ECS::Transform*>(EditorLayer::scene->getEntites().at(0));

    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_W)) transform1->transform += glm::vec3(0.0f, 0.0f, -0.01f);
    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_A)) transform1->transform += glm::vec3(-0.01f, 0.0f, 0.0f);
    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_S)) transform1->transform += glm::vec3(0.0f, 0.0f, 0.01f);
    if(GCGameEngine::Input::isKeyPressed(SDL_SCANCODE_D)) transform1->transform += glm::vec3(0.01f, 0.0f, 0.0f);
}