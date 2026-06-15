#ifndef RENDERER_HANDLE_H
#define RENDERER_HANDLE_H

#include <cstdint>

#include <glm/glm.hpp>

namespace E64{
    using CommandBufferHandle = uint32_t;

    using GPUBufferHandle = uint32_t;
    using GPUTextureHandle = uint32_t;
    using GPUSamplerHandle = uint32_t;

    struct __attribute__((packed)) PackedVec3 {
        float x, y, z;
        PackedVec3() : x(0), y(0), z(0) {}
        PackedVec3(float x, float y, float z) : x(x), y(y), z(z) {}
        PackedVec3(std::initializer_list<float> l) {
            auto it = l.begin();
            x = *it++; y = *it++; z = *it++;
        }
    };
    
    struct __attribute__((packed)) PackedVec4 {
        float r, g, b, a;
        PackedVec4() : r(0), g(0), b(0), a(0) {}
        PackedVec4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        PackedVec4(std::initializer_list<float> l) {
            auto it = l.begin();
            r = *it++; g = *it++; b = *it++; a = *it++;
        }
    };
    
    struct __attribute__((packed)) PackedVec2 {
        float x, y;
        PackedVec2() : x(0), y(0) {}
        PackedVec2(float x, float y) : x(x), y(y) {}
        PackedVec2(std::initializer_list<float> l) {
            auto it = l.begin();
            x = *it++; y = *it++;
        }
    };
    
    struct __attribute__((packed)) Vertex{
        PackedVec3 pos;
        PackedVec4 color;
        PackedVec2 uv;
    };

    using Index = uint32_t;
}

#endif