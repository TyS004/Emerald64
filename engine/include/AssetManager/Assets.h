#ifndef ASSETS_H
#define ASSETS_H

#include "Renderer/RendererHandle.h"
#include <fstream>
#include <stdint.h>
#include <cstring>
#include "stb_image.h"
#include "Logger/Logger.h"

namespace E64{

    struct MeshBatch{
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        int base;
    };

    struct Mesh{
        GPUBufferHandle vbo = -1;
        GPUBufferHandle ibo = -1;

        std::vector<Vertex> vertices = { 
            // Front (normal: 0, 0, 1)
            {{-0.5f, -0.5f,  0.5f}, {1,0,1,1}, {0,0}, {0,0,1}},
            {{ 0.5f, -0.5f,  0.5f}, {1,0,1,1}, {1,0}, {0,0,1}},
            {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,1}, {0,0,1}},
            {{-0.5f, -0.5f,  0.5f}, {1,0,1,1}, {0,0}, {0,0,1}},
            {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,1}, {0,0,1}},
            {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,1}, {0,0,1}},
            // Back (normal: 0, 0, -1)
            {{ 0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {0,0,-1}},
            {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {1,0}, {0,0,-1}},
            {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {0,0,-1}},
            {{ 0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {0,0,-1}},
            {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {0,0,-1}},
            {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {0,1}, {0,0,-1}},
            // Left (normal: -1, 0, 0)
            {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {-1,0,0}},
            {{-0.5f, -0.5f,  0.5f}, {1,0,1,1}, {1,0}, {-1,0,0}},
            {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,1}, {-1,0,0}},
            {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {-1,0,0}},
            {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,1}, {-1,0,0}},
            {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {0,1}, {-1,0,0}},
            // Right (normal: 1, 0, 0)
            {{ 0.5f, -0.5f,  0.5f}, {1,0,1,1}, {0,0}, {1,0,0}},
            {{ 0.5f, -0.5f, -0.5f}, {1,0,1,1}, {1,0}, {1,0,0}},
            {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {1,0,0}},
            {{ 0.5f, -0.5f,  0.5f}, {1,0,1,1}, {0,0}, {1,0,0}},
            {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {1,0,0}},
            {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,1}, {1,0,0}},
            // Top (normal: 0, 1, 0)
            {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}, {0,1,0}},
            {{ 0.5f,  0.5f,  0.5f}, {1,0,1,1}, {1,0}, {0,1,0}},
            {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {0,1,0}},
            {{-0.5f,  0.5f,  0.5f}, {1,0,1,1}, {0,0}, {0,1,0}},
            {{ 0.5f,  0.5f, -0.5f}, {1,0,1,1}, {1,1}, {0,1,0}},
            {{-0.5f,  0.5f, -0.5f}, {1,0,1,1}, {0,1}, {0,1,0}},
            // Bottom (normal: 0, -1, 0)
            {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {0,-1,0}},
            {{ 0.5f, -0.5f, -0.5f}, {1,0,1,1}, {1,0}, {0,-1,0}},
            {{ 0.5f, -0.5f,  0.5f}, {1,0,1,1}, {1,1}, {0,-1,0}},
            {{-0.5f, -0.5f, -0.5f}, {1,0,1,1}, {0,0}, {0,-1,0}},
            {{ 0.5f, -0.5f,  0.5f}, {1,0,1,1}, {1,1}, {0,-1,0}},
            {{-0.5f, -0.5f,  0.5f}, {1,0,1,1}, {0,1}, {0,-1,0}},
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

        std::vector<MeshBatch> batches{};
        int MAX_BATCH_SIZE = 36;

        void batch(){
            for(size_t i = 0; i < vertices.size(); i += MAX_BATCH_SIZE){
                MeshBatch batch;
                batch.base = i;
                int batch_size = std::min(MAX_BATCH_SIZE, (int)vertices.size() - (int)i);
                int k = 0;
                for(size_t j = i; j < batch_size + i; ++j){
                    batch.vertices.push_back(vertices.at(indices.at(j)));
                    batch.indices.push_back(k);
                    ++k;
                }
                batches.push_back(batch);
                E64::Log::info("Batch Base" + std::to_string(i) + ": Indices Size" + std::to_string(batch.indices.size()));
                E64::Log::info("Batch Base" + std::to_string(i) + ": Vetex Size" + std::to_string(batch.vertices.size()));
            }
            E64::Log::info("Created " + std::to_string(batches.size()) + " batches");
        }
    };

    struct Texture{
        GPUTextureHandle texture = -1;
        GPUSamplerHandle sampler = -1;

        int width;
        int height;
        int channels;

        unsigned char* img_data = stbi_load("../assets/textures/test.png", &width, &height, &channels, STBI_rgb_alpha);
    };
}

#endif