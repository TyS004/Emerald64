#ifndef ECS_H
#define ECS_H

#include "Renderer/VBO.h"
#include "Renderer/IBO.h"

namespace E64{
    namespace ECS{
        using ComponetMask = uint32_t;

        constexpr ComponetMask TRANSFORM = 1 << 0; // 0001
        constexpr ComponetMask MESH      = 1 << 1; // 0010

        struct TransformComponet{
            glm::vec3 position = {0, 0, 0};
            glm::vec3 euler = {0, 0, 0};
            glm::vec3 scale = {1, 1 , 1};
        };
       
        struct MeshComponet{
            uint32_t id;
        };
        
        struct Mesh{
            VBO* vbo;
            IBO* ibo;
            std::string path;
        };
    }
}

#endif