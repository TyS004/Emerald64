#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "ECS/Componet.h"

namespace E64{
    class AssetManager{
        public:
            AssetManager();

            uint32_t addMesh(ECS::Mesh* mesh);
            ECS::Mesh* getMesh(uint32_t id);
            std::vector<ECS::Mesh> getMeshes();

        private:
            std::vector<ECS::Mesh> mesh_repository;
    };
}

#endif