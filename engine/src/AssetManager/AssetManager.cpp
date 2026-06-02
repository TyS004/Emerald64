#include "AssetManager/AssetManager.h"
#include "Engine.h"

#include <filesystem>

int mesh_id = 0;

E64::AssetManager::AssetManager(){
    E64::Log::info("ASSET MANAGER INSTANTIATED\n");

    ECS::Mesh default_mesh{};
    addMesh(default_mesh);
}

E64::AssetManager::~AssetManager(){
    
}

E64::ECS::MeshComponent E64::AssetManager::addMesh(E64::ECS::Mesh mesh){
    mesh.vbo_handle = E64::Engine::ctx->renderer->createVertexBuffer(mesh.vertices);
    mesh.ibo_handle = E64::Engine::ctx->renderer->createIndexBuffer(mesh.indices);
    mesh.texture_handle = E64::Engine::ctx->renderer->createTexture(mesh.texture_path);
    mesh.sampler_handle = E64::Engine::ctx->renderer->createSampler();

    ECS::MeshComponent comp;
    comp.mesh_handle.path = mesh.obj_path;
    comp.mesh_handle.id = mesh_id++;

    E64::Log::debug("REGISTERED MESH: " + mesh.obj_path + " TO ASSETMANAGER");
    mesh_repository[comp.mesh_handle.path] = std::make_unique<ECS::Mesh>(std::move(mesh));

    return comp;
}

E64::ECS::Mesh* E64::AssetManager::getMesh(E64::AssetHandle handle){
    try { return mesh_repository.at(handle.path).get(); }
    catch(std::out_of_range e) { E64::Log::error("Asset not Loaded Into Asset Map"); return nullptr; }
}

std::vector<E64::ECS::Mesh*> E64::AssetManager::getMeshes(){
    std::vector<ECS::Mesh*> meshes;
    meshes.reserve(mesh_repository.size());

    for (auto& [path, meshPtr] : mesh_repository)
        meshes.push_back(meshPtr.get());

    return meshes;
}