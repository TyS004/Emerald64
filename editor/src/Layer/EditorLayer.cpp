#include "Layer/EditorLayer.h"
#include "EditorInput/EditorInput.h"

using namespace GCGameEngine;
using namespace ECS;

GCGameEngine::Scene* Editor::EditorLayer::scene = new GCGameEngine::Scene();
FBXParser* Editor::EditorLayer::parser = new FBXParser("../assets/meshes/tinker.obj");

Editor::EditorLayer::EditorLayer(){    
    Entity e1 = GCGameEngine::ECS::EntityManager::createEntity();
    scene->pushEntity(&e1);

    GCGameEngine::Engine::setActiveScene(scene);

    std::vector<GCGameEngine::ECS::Entity> entities = scene->getEntites();
    
    GCGameEngine::ECS::Transform* transform = new Transform{};
    GCGameEngine::ECS::Mesh* obj_mesh = parser->getMesh();
    GCGameEngine::ECS::ComponetManager::addComponet<Transform*>(entities[0], transform);
    GCGameEngine::ECS::ComponetManager::addComponet<Mesh*>(entities[0], obj_mesh);

    GCGameEngine::Input::OnKeyPressedBind = Editor::EditorInput::OnKeyPressed;
    GCGameEngine::Input::OnKeyDownBind = Editor::EditorInput::OnKeyDown;
    GCGameEngine::Input::OnMouseMoveBind = Editor::EditorInput::OnMouseMove;
}

Editor::EditorLayer::~EditorLayer(){
    delete scene;
    delete parser;
}

void Editor::EditorLayer::OnUpdate(){
    Camera* camera = scene->getCamera();
    camera->OnUpdate();
}