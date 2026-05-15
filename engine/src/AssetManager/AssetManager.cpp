#include "AssetManager/AssetManager.h"

int mesh_id = 0;

E64::AssetManager::AssetManager(){
    mesh_repository = {};
}

uint32_t E64::AssetManager::addMesh(E64::ECS::Mesh mesh){
    mesh_repository.push_back(mesh);
    return mesh_id++;
}

E64::ECS::Mesh* E64::AssetManager::getMesh(uint32_t id){
    return &mesh_repository[id];
}

std::vector<E64::ECS::Mesh> E64::AssetManager::getMeshes(){
    return mesh_repository;
}