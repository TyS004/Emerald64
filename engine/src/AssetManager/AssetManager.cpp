#include "AssetManager/AssetManager.h"
#include "Serialization/MeshSerializer.h"
#include "Engine.h"

#include <filesystem>

int mesh_id = 0;
int tex_id = 0;

E64::AssetManager::AssetManager(){
    E64::Log::info("ASSET MANAGER INSTANTIATED\n");

    loadMeshAsset("default");
    loadTextureAsset("default");
}

E64::AssetManager::~AssetManager(){
    
}

E64::AssetHandle E64::AssetManager::loadMeshAsset(std::string path){
    if(mesh_handle_repository.count(path) > 0){
        E64::Log::info("Mesh Already Registered");
        return mesh_handle_repository.at(path);
    }
    
    E64::Mesh mesh{};
    if(path != "default"){
        std::filesystem::path absolute_path = E64::Engine::ctx->root_dir.string() + path;
        E64::MeshSerializer serializer;
        mesh = serializer.deserialize(absolute_path);
    }
    mesh.batch();

    mesh.vbo = E64::Engine::ctx->renderer->createVertexBuffer(mesh.vertices);
    mesh.ibo = E64::Engine::ctx->renderer->createIndexBuffer(mesh.indices);

    AssetHandle handle = mesh_id;
    mesh_id++;

    E64::Log::debug("REGISTERED MESH: " + path + " TO ASSETMANAGER");
    mesh_repository[handle] = std::make_unique<Mesh>(std::move(mesh));
    mesh_handle_repository[path] = handle;
    return handle;
}

E64::AssetHandle E64::AssetManager::loadTextureAsset(std::string path){
    if(texture_handle_repository.count(path) > 0){
        E64::Log::info("Texture Already Registered");
        return texture_handle_repository.at(path);
    }

    Texture texture{};
    if(path != "default"){
        std::filesystem::path absolute_path = E64::Engine::ctx->root_dir.string() + path;
        texture.img_data = stbi_load(absolute_path.c_str(), &texture.width, &texture.height, &texture.channels, STBI_rgb_alpha);
    }
    texture.texture = E64::Engine::ctx->renderer->createTexture(texture.img_data, texture.width, texture.height);
    texture.sampler = E64::Engine::ctx->renderer->createSampler();

    AssetHandle handle = tex_id++;

    E64::Log::debug("REGISTERED TEXTURE: " + path + " TO ASSETMANAGER");
    texture_repository[handle] = std::make_unique<Texture>(std::move(texture));
    texture_handle_repository[path] = handle;

    return handle;
}

E64::Mesh* E64::AssetManager::getMeshAsset(AssetHandle handle){
    try { return mesh_repository.at(handle).get(); }
    catch(std::out_of_range e) { E64::Log::error("Mesh Asset not Loaded Into Asset Map"); return nullptr; }
}

E64::Texture* E64::AssetManager::getTextureAsset(AssetHandle handle){
    try { return texture_repository.at(handle).get(); }
    catch(std::out_of_range e) { E64::Log::error("Texture Asset not Loaded Into Asset Map"); return nullptr; }
}

std::vector<E64::Mesh*> E64::AssetManager::getAssets(){
    std::vector<E64::Mesh*> meshes;
    meshes.reserve(mesh_repository.size());

    for (auto& [handle, mesh_ptr] : mesh_repository)
        meshes.push_back(mesh_ptr.get());

    return meshes;
}

std::vector<E64::AssetHandle> E64::AssetManager::getHandles(){
    std::vector<E64::AssetHandle> handles;
    handles.reserve(mesh_handle_repository.size());

    for(auto& [path, handle] : mesh_handle_repository){
        handles.push_back(handle);
    }
    return handles;
}

std::unordered_map<std::string, E64::AssetHandle> E64::AssetManager::getHandleRepository(){
    return mesh_handle_repository;
}