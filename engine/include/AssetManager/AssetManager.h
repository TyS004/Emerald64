#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "AssetHandle.h"
#include "ECS/Component.h"

namespace E64{
    class AssetManager{
        public:
            AssetManager();
            ~AssetManager();

            ECS::MeshComponent addMesh(ECS::Mesh mesh);
            ECS::Mesh* getMesh(AssetHandle handle);
            std::vector<ECS::Mesh*> getMeshes();

        private:
            std::unordered_map<std::string, std::unique_ptr<ECS::Mesh>> mesh_repository;
            std::string path;
    };
}

#endif