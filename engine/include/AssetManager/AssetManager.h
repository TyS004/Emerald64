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

            ECS::MeshComponent addMesh(Mesh mesh);
            Mesh* getMesh(AssetHandle handle);
            std::vector<Mesh*> getMeshes();

        private:
            std::unordered_map<std::string, std::unique_ptr<Mesh>> mesh_repository;
            std::unordered_map<std::string, std::unique_ptr<Texture>> texture_reposity;
    };
}

#endif