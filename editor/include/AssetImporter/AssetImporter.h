#ifndef ASSET_IMPORTER_H
#define ASSET_IMPORTER_H

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <E64.h>

namespace Editor{
    class AssetImporter{
        public:
            AssetImporter();
            ~AssetImporter();

            E64::Mesh importMesh(std::string path);
    };
}

#endif 