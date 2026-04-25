#ifndef VBO_H
#define VBO_H

#include "SDL3/SDL.h"

namespace GCGameEngine{

    struct vec3{
        float x, y, z;
    };

    struct vec4{
        float x, y, z, w;
    };

    struct Vertex{
        vec3 pos;
        vec4 color;
    };

    class VBO{
        public:
            VBO(const uint16_t num_vertices);
            ~VBO();

            SDL_GPUBufferBinding* getBufferBinding();
        private:
            uint16_t num_vertices;
            Vertex* vertices;

            SDL_GPUDevice* device;
            SDL_GPUBuffer* vertex_buffer;
            SDL_GPUTransferBuffer* transfer_buffer;
            SDL_GPUBufferBinding* buffer_binding;
    };
}

#endif 