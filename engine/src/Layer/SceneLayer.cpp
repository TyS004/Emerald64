#include "Layer/SceneLayer.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include "AssetManager/AssetManager.h"

#include "Serialization/SceneSerializer.h"
#include "Serialization/MeshSerializer.h"

#include <cstdlib>
#include <ctime>
#include <random>

E64::SceneLayer::SceneLayer(){
    ECS::ComponentRegistry<ECS::TransformComponent>::registerComponent("Transform");
    ECS::ComponentRegistry<ECS::MeshComponent>::registerComponent("Mesh");
    ECS::ComponentRegistry<ECS::CameraComponent>::registerComponent("Camera");

    E64::Engine::ctx->asset_manager = std::make_unique<E64::AssetManager>();
    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>();

    if(E64::Engine::ctx->mode == EDITOR || E64::Engine::ctx->mode == N64_RUNTIME){
        scene = E64::Engine::ctx->active_scene.get();
        scene->createDefaultScene();
    }
    else if(E64::Engine::ctx->mode == DESKTOP_RUNTIME){
        //Deserialize at Runtime

        //Scene
        SceneSerializer scene_serializer;
        scene = scene_serializer.deserialize();
        if(!scene){ E64::Log::error("Scene Could Not Be Loaded"); }
        else{ E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(*scene)); }

        //Meshes
        MeshSerializer  mesh_serializer;
        for (const auto& entry : std::filesystem::directory_iterator("../assets/meshes/")) {
            if(entry.path().extension() == ".e64mesh")
            {
                ECS::Mesh mesh = mesh_serializer.deserialize(entry.path().string());
                E64::Engine::ctx->asset_manager->addMesh(mesh);
            }
        }
    }
}

E64::SceneLayer::~SceneLayer(){

}

void E64::SceneLayer::OnRender(){
    scene = E64::Engine::ctx->active_scene.get();
    if(!scene) { E64::Log::error("NO SCENE"); return; }

    E64::IRenderer* renderer = E64::Engine::ctx->renderer;

    if(E64::Engine::ctx->mode == EDITOR){
        renderer->beginRenderPass(RenderTarget::TEXTURE);
    }
    else{
        renderer->beginRenderPass(RenderTarget::SWAPCHAIN);
    }
    renderer->bindPipeline();
    scene->render();
    renderer->endRenderPass();
}