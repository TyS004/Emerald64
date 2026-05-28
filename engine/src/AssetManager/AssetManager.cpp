#include "AssetManager/AssetManager.h"
#include <filesystem>

int mesh_id = 0;

E64::AssetManager::AssetManager(){
    E64::Log::info("ASSET MANAGER INSTANTIATED");
    E64::Log::info("LOADED DEFAULT MESH");

    ECS::Mesh default_mesh{};
    ECS::MeshComponent mesh_comp = { addMesh(default_mesh) };
}

E64::AssetManager::~AssetManager(){
    
}

E64::AssetHandle E64::AssetManager::addMesh(E64::ECS::Mesh& mesh){
    mesh.upload();

    E64::AssetHandle handle;
    handle.path = mesh.path;
    handle.id = mesh_id++;

    mesh_repository[handle.path] = std::make_unique<ECS::Mesh>(std::move(mesh));
    E64::Log::debug("Registerd Mesh: " + mesh.path.string() + " To AssetManager");

    return handle;
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