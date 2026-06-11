#ifndef ECS_H
#define ECS_H

#include "Renderer/RendererHandle.h"
#include "Logger/Logger.h"

#include <fstream>

#include "AssetManager/AssetHandle.h"

#include <json.hpp>

using json = nlohmann::json;

namespace E64{
    namespace ECS{
        struct TransformComponent{
            glm::vec3 position =  {0, 0, 0};
            glm::vec3 euler    =  {0, 0, 0};
            glm::vec3 scale    =  {1, 1, 1};

            void serialize(json& j) const
            {
                j["position"] = {position.x, position.y, position.z};
                j["rotation"] = {euler.x, euler.y, euler.z};
                j["scale"]    = {scale.x, scale.y, scale.z};
            }

            void deserialize(const json& j)
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
        
        struct MeshComponent{
            AssetHandle mesh_handle;
            AssetHandle tex_handle;
            
            void serialize(json& j) const
            {
                j["mesh_handle_id"] = {mesh_handle.id};
                j["mesh_handle_path"] = {mesh_handle.path};
                j["tex_handle_id"] = {tex_handle.id};
                j["tex_handle_path"] = {tex_handle.path};
            } 

            void deserialize(const json& j)
            {
                mesh_handle.id = j["mesh_handle_id"][0];
                mesh_handle.path = j["mesh_handle_path"][0];
                tex_handle.id = j["tex_handle_id"][0];
                tex_handle.path = j["tex_handle_path"][0];
            }
        };

        struct CameraComponent{
            float fov;
            float aspect_ratio;
            float near_plane;
            float far_plane;

            void serialize(json& j) const
            {
                j["fov"] = fov;
                j["aspect_ratio"] = aspect_ratio;
                j["near_plane"] = near_plane;
                j["far_plane"] = far_plane;
            }

            void deserialize(const json& j)
            {
                fov = j["fov"];
                aspect_ratio = j["aspect_ratio"];
                near_plane = j["near_plane"];
                far_plane = j["far_plane"];
            }
        };

        struct CameraData{
            glm::mat4 proj;
            glm::mat4 view;
        };

        using ComponentMask = uint32_t;
        template<typename T> struct ComponentBit;
        template<> struct ComponentBit<TransformComponent> { static constexpr ComponentMask mask = 1 << 0; }; // 0001
        template<> struct ComponentBit<MeshComponent>      { static constexpr ComponentMask mask = 1 << 1; }; // 0010
        template<> struct ComponentBit<CameraComponent>    { static constexpr ComponentMask mask = 1 << 2; }; // 0100
    }
}

#endif