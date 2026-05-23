#ifndef ECS_H
#define ECS_H

#include "Renderer/VBO.h"
#include "Renderer/IBO.h"
#include "Renderer/Texture.h"
#include <json.hpp>

using json = nlohmann::json;

namespace E64{
    namespace ECS{
        using ComponetMask = uint32_t;

        constexpr ComponetMask TRANSFORM = 1 << 0; // 0001
        constexpr ComponetMask MESH      = 1 << 1; // 0010

        struct TransformComponet{
            glm::vec3 position = {0, 0, 0};
            glm::vec3 euler = {0, 0, 0};
            glm::vec3 scale = {1, 1, 1};

            void serialize(json& j) const
            {
                j["position"] = {position.x, position.y, position.z};
                j["rotation"] = {euler.x, euler.y, euler.z};
                j["scale"]    = {scale.x, scale.y, scale.z};
            }

            void deserialize(json& j)
            {
                position.x = j["position"][0];
                position.y = j["position"][1];
                position.z = j["position"][2];

                euler.x = j["rotation"][0];
                euler.y = j["rotation"][1];
                euler.z = j["rotation"][2];

                scale.x = j["scale"][0];
                scale.y = j["scale"][1];
                scale.z = j["scale"][2];
            }
        };
       
        struct MeshComponet{
            uint32_t id;
            
            void serialize(json& j) const
            {
                j["asset_id"] = {id};
            }

            void deserialize(json& j)
            {
                id = j["asset_id"][0];
            }
        };
        
        struct Mesh{
            VBO* vbo;
            IBO* ibo;
            Texture* texture;
            std::string path;
        };
    }
}

#endif