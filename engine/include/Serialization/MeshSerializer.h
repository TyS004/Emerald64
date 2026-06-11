#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

#include "AssetManager/Assets.h"

namespace E64{
    class MeshSerializer{
        public:
            MeshSerializer();
            ~MeshSerializer();
        
        void serialize(E64::Mesh* mesh);
        Mesh deserialize(std::string path);
    };
}

#endif