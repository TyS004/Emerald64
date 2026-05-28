#ifndef ECS_H
#define ECS_H

#include "Renderer/VBO.h"
#include "Renderer/IBO.h"
#include "Renderer/TBO.h"
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

        struct Mesh{
            VBO vbo;
            IBO ibo;
            TBO texture;

            std::vector<Vertex> vertices = { 
                // Front (red)
                {{-0.5f, -0.5f,  0.5f}, {1,0,0,1}, {0,0}},
                {{ 0.5f, -0.5f,  0.5f}, {1,0,0,1}, {1,0}},
                {{ 0.5f,  0.5f,  0.5f}, {1,0,0,1}, {1,1}},
                {{-0.5f, -0.5f,  0.5f}, {1,0,0,1}, {0,0}},
                {{ 0.5f,  0.5f,  0.5f}, {1,0,0,1}, {1,1}},
                {{-0.5f,  0.5f,  0.5f}, {1,0,0,1}, {0,1}},
            
                // Back (green)
                {{ 0.5f, -0.5f, -0.5f}, {0,1,0,1}, {0,0}},
                {{-0.5f, -0.5f, -0.5f}, {0,1,0,1}, {1,0}},
                {{-0.5f,  0.5f, -0.5f}, {0,1,0,1}, {1,1}},
                {{ 0.5f, -0.5f, -0.5f}, {0,1,0,1}, {0,0}},
                {{-0.5f,  0.5f, -0.5f}, {0,1,0,1}, {1,1}},
                {{ 0.5f,  0.5f, -0.5f}, {0,1,0,1}, {0,1}},
            
                // Left (blue)
                {{-0.5f, -0.5f, -0.5f}, {0,0,1,1}, {0,0}},
                {{-0.5f, -0.5f,  0.5f}, {0,0,1,1}, {1,0}},
                {{-0.5f,  0.5f,  0.5f}, {0,0,1,1}, {1,1}},
                {{-0.5f, -0.5f, -0.5f}, {0,0,1,1}, {0,0}},
                {{-0.5f,  0.5f,  0.5f}, {0,0,1,1}, {1,1}},
                {{-0.5f,  0.5f, -0.5f}, {0,0,1,1}, {0,1}},
            
                // Right (yellow)
                {{ 0.5f, -0.5f,  0.5f}, {1,1,0,1}, {0,0}},
                {{ 0.5f, -0.5f, -0.5f}, {1,1,0,1}, {1,0}},
                {{ 0.5f,  0.5f, -0.5f}, {1,1,0,1}, {1,1}},
                {{ 0.5f, -0.5f,  0.5f}, {1,1,0,1}, {0,0}},
                {{ 0.5f,  0.5f, -0.5f}, {1,1,0,1}, {1,1}},
                {{ 0.5f,  0.5f,  0.5f}, {1,1,0,1}, {0,1}},
            
                // Top (magenta)
                {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}},
                {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,0}},
                {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}},
                {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}},
                {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}},
                {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {0,1}},
            
                // Bottom (cyan)
                {{-0.5f, -0.5f, -0.5f}, {0,1,1,1}, {0,0}},
                {{ 0.5f, -0.5f, -0.5f}, {0,1,1,1}, {1,0}},
                {{ 0.5f, -0.5f,  0.5f}, {0,1,1,1}, {1,1}},
                {{-0.5f, -0.5f, -0.5f}, {0,1,1,1}, {0,0}},
                {{ 0.5f, -0.5f,  0.5f}, {0,1,1,1}, {1,1}},
                {{-0.5f, -0.5f,  0.5f}, {0,1,1,1}, {0,1}}
            };

            std::vector<uint32_t> indices = {
                // Front
                0, 1, 2,
                3, 4, 5,
        
                // Back
                6, 7, 8,
                9, 10, 11,
        
                // Left
                12, 13, 14,
                15, 16, 17,
        
                // Right
                18, 19, 20,
                21, 22, 23,
        
                // Top
                24, 25, 26,
                27, 28, 29,
        
                // Bottom
                30, 31, 32,
                33, 34, 35
            };

            std::string path = "default";

            void upload(){
                vbo.upload(vertices);
                ibo.upload(indices);
                texture.upload();
            }

            friend std::ofstream& operator<<(std::ofstream& file, const ECS::Mesh& mesh) {
                E64::Log::info("Writing to E64Mesh..." + mesh.path);
            
                uint32_t vcount   = mesh.vertices.size();
                uint32_t icount   = mesh.indices.size();
                std::string path_str = mesh.path;
                uint32_t path_len = path_str.size();
            
                file.write(reinterpret_cast<const char*>(&vcount),               sizeof(uint32_t));
                file.write(reinterpret_cast<const char*>(mesh.vertices.data()),  sizeof(Vertex) * vcount);
                file.write(reinterpret_cast<const char*>(&icount),               sizeof(uint32_t));
                file.write(reinterpret_cast<const char*>(mesh.indices.data()),   sizeof(uint32_t) * icount);
                file.write(reinterpret_cast<const char*>(&path_len),             sizeof(uint32_t));
                file.write(path_str.c_str(),                                     path_len);
            
                return file;
            }

            friend std::ifstream& operator>>(std::ifstream& file, ECS::Mesh& mesh) {
                E64::Log::info("Reading from E64Mesh..." + mesh.path);

                uint32_t vcount = 0;
                file.read(reinterpret_cast<char*>(&vcount), sizeof(uint32_t));
                mesh.vertices.resize(vcount);
                file.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(Vertex) * vcount);


                uint32_t icount = 0;
                file.read(reinterpret_cast<char*>(&icount), sizeof(uint32_t));
                mesh.indices.resize(icount);
                file.read(reinterpret_cast<char*>(mesh.indices.data()), sizeof(uint32_t) * icount);


                uint32_t path_len = 0;
                file.read(reinterpret_cast<char*>(&path_len), sizeof(uint32_t));
                std::string path_str(path_len, '\0');
                file.read(path_str.data(), path_len);
                mesh.path = path_str;

                return file;
            }
        };

        struct Texture{
            TBO texture;
            const char* img_data;
        };

        using ComponentMask = uint32_t;
        template<typename T> struct ComponentBit;
        template<> struct ComponentBit<TransformComponent> { static constexpr ComponentMask mask = 1 << 0; }; // 0001
        template<> struct ComponentBit<MeshComponent>      { static constexpr ComponentMask mask = 1 << 1; }; // 0010
        template<> struct ComponentBit<CameraComponent>    { static constexpr ComponentMask mask = 1 << 2; }; // 0100
    }
}

#endif