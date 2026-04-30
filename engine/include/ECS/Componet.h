#ifndef ECS_H
#define ECS_H

#include "Renderer/VBO.h"

namespace GCGameEngine{
    namespace ECS{
        using ComponetMask = uint32_t;

        constexpr ComponetMask TRANSFORM = 1 << 0; // 0001
        constexpr ComponetMask MESH      = 1 << 1; // 0010

        struct Transform{
            glm::vec3 position;
            glm::vec3 euler;
            glm::vec3 scale;
        };
        
        struct Mesh{
            VBO* vbo;
        };
    }
}

#endif