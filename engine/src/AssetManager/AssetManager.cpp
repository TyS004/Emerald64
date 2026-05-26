#include "AssetManager/AssetManager.h"
#include <filesystem>

int mesh_id = 0;

E64::AssetManager::AssetManager(){
    mesh_repository = {};
    E64::Log::info("ASSET MANAGER INSTANTIATED");
    E64::Log::info("LOADED DEFAULT MESH");

    VBO* vbo = new VBO();
    IBO* ibo = new IBO();
    Texture* texture = new Texture();

    ECS::Mesh obj_mesh = ECS::Mesh{ vbo, ibo, texture, "default"};
    ECS::MeshComponent mesh_comp = { addMesh(obj_mesh) };
}

E64::AssetManager::~AssetManager(){
    for(const std::pair<std::string, ECS::Mesh>& pair : mesh_repository){
        delete pair.second.vbo;
        delete pair.second.ibo;
        delete pair.second.texture;
    }
}

E64::AssetHandle E64::AssetManager::addMesh(E64::ECS::Mesh mesh){
    mesh.vbo->upload();
    mesh.ibo->upload();
    mesh.texture->upload();

    E64::AssetHandle handle;
    handle.path = mesh.path;
    std::cout << mesh_id << std::endl;
    handle.id = mesh_id++;

    mesh_repository[handle.path] = mesh;
    return handle;
}

E64::ECS::Mesh* E64::AssetManager::getMesh(E64::AssetHandle handle){
    try { return &mesh_repository.at(handle.path); }
    catch(std::out_of_range e) { return nullptr; }
}

std::vector<E64::ECS::Mesh> E64::AssetManager::getMeshes(){
    std::vector<E64::ECS::Mesh> meshes;
    mesh_repository.reserve(mesh_repository.size());
    
    for(const std::pair<std::string, ECS::Mesh>& pair : mesh_repository){
        meshes.push_back(pair.second);
    }

    return meshes;
}