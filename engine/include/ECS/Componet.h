#ifndef ECS_H
#define ECS_H

#include "Renderer/VBO.h"

namespace GCGameEngine{
    namespace Componet{
        struct Transform{
            glm::vec3 transform;
        };
        
        struct Mesh{
            VBO* vbo;
        };
    }
}

#endif