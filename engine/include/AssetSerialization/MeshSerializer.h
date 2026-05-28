#ifndef MESH_SERIALIZER_H
#define MESH_SERIALIZER_H

#include "ECS/Component.h"

namespace E64{
    class MeshSerializer{
        public:
            MeshSerializer();
            ~MeshSerializer();
        
        void serialize(E64::ECS::Mesh* mesh);
    };
}

#endif