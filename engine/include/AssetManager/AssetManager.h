#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "AssetHandle.h"
#include "AssetManager/Assets.h"
#include "ECS/Component.h"

namespace E64{
    class AssetManager{
        public:
            AssetManager();
            ~AssetManager();

            AssetHandle loadMeshAsset(std::string path);
            AssetHandle loadTextureAsset(std::string path);
            
            Mesh* getMeshAsset(AssetHandle handle);
            Texture* getTextureAsset(AssetHandle handle);

            std::vector<Mesh*> getAssets();
            std::vector<AssetHandle> getHandles();
            std::unordered_map<std::string, E64::AssetHandle> getHandleRepository();

        private:
            std::unordered_map<AssetHandle, std::unique_ptr<Mesh>> mesh_repository;
            std::unordered_map<std::string, AssetHandle> mesh_handle_repository;

            std::unordered_map<AssetHandle, std::unique_ptr<Texture>> texture_repository;
            std::unordered_map<std::string, AssetHandle> texture_handle_repository;

            std::filesystem::path project_dir;
    };
}

#endif