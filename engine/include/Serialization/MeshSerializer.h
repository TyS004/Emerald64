#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

#include "AssetManager/Assets.h"
#include "ECS/Component.h"

namespace E64{
    class MeshSerializer{
        public:
            MeshSerializer();
            ~MeshSerializer();
        
        void serialize(E64::Mesh mesh, std::string path);
        Mesh deserialize(std::string path);
    };
}

#endif