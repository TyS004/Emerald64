#ifndef RENDERER_HANDLE_H
#define RENDERER_HANDLE_H

#include <cstdint>

#include <glm/glm.hpp>

namespace E64{
    using CommandBufferHandle = uint32_t;

    using GPUBufferHandle = uint32_t;
    using GPUTextureHandle = uint32_t;
    using GPUSamplerHandle = uint32_t;
    
    struct Vertex{
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 uv;
    };

    using Index = uint32_t;
}

#endif