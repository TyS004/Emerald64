#include "Layer/EditorLayer.h"
#include "EditorInput/EditorInput.h"

using namespace GCGameEngine;
using namespace ECS;

GCGameEngine::Scene* Editor::EditorLayer::scene = new GCGameEngine::Scene();

Editor::EditorLayer::EditorLayer(){
    Entity e1 = GCGameEngine::ECS::EntityManager::createEntity();
    scene->pushEntity(&e1);

    GCGameEngine::Engine::setActiveScene(scene);

    std::vector<GCGameEngine::ECS::Entity> entities = scene->getEntites();
    
    GCGameEngine::ECS::ComponetManager::addComponet<Transform*>(entities[0]);
    GCGameEngine::ECS::ComponetManager::addComponet<Mesh*>(entities[0]);

    GCGameEngine::Input::OnKeyPressedBind = Editor::EditorInput::OnKeyPressed;
    GCGameEngine::Input::OnKeyDownBind = Editor::EditorInput::OnKeyDown;
    GCGameEngine::Input::OnMouseMoveBind = Editor::EditorInput::OnMouseMove;
}

Editor::EditorLayer::~EditorLayer(){
    delete scene;
    GCGameEngine::ECS::ComponetManager::clean();
}

void Editor::EditorLayer::OnUpdate(){
    Camera* camera = scene->getCamera();
    camera->OnUpdate();
}