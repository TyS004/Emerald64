#ifndef ASSETS_H
#define ASSETS_H

#include "Renderer/RendererHandle.h"
#include <fstream>
#include "Logger/Logger.h"

namespace E64{
    struct Mesh{
        GPUBufferHandle vbo_handle        = -1;
        GPUBufferHandle ibo_handle        = -1;
        GPUTextureHandle texture_handle   = -1;
        GPUSamplerHandle sampler_handle   = -1;

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

        std::string obj_path = "default";
        std::string texture_path = "";

        friend std::ofstream& operator<<(std::ofstream& file, const Mesh& mesh) {
            E64::Log::info("Writing to E64Mesh..." + mesh.obj_path);
        
            uint32_t vcount   = mesh.vertices.size();
            uint32_t icount   = mesh.indices.size();
            std::string path_str = mesh.obj_path;
            uint32_t path_len = path_str.size();
        
            file.write(reinterpret_cast<const char*>(&vcount),               sizeof(uint32_t));
            file.write(reinterpret_cast<const char*>(mesh.vertices.data()),  sizeof(Vertex) * vcount);
            file.write(reinterpret_cast<const char*>(&icount),               sizeof(uint32_t));
            file.write(reinterpret_cast<const char*>(mesh.indices.data()),   sizeof(uint32_t) * icount);
            file.write(reinterpret_cast<const char*>(&path_len),             sizeof(uint32_t));
            file.write(path_str.c_str(),                                     path_len);
        
            return file;
        }

        friend std::ifstream& operator>>(std::ifstream& file, Mesh& mesh) {
            E64::Log::info("Reading from E64Mesh..." + mesh.obj_path);

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
            mesh.obj_path = path_str;

            return file;
        }
    };

    struct Texture{
        GPUTextureHandle texture = -1;
        GPUSamplerHandle sampler = -1;

        const char* img_data = nullptr;
    };
}

#endif