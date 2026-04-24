#ifndef VBO_H
#define VBO_H

#include "SDL3/SDL.h"

namespace GCGameEngine{
    struct Vertex{
        float x, y, z;
        float r, g, b, a; 
    };

    class VBO{
        public:
            VBO(SDL_GPUDevice* device, const uint16_t num_vertices);
            ~VBO();

            SDL_GPUBufferBinding* getBufferBinding();
        private:
            uint16_t num_vertices;
            Vertex* vertices;

            SDL_GPUDevice* device;
            SDL_GPUBuffer* vertex_buffer;
            SDL_GPUTransferBuffer* transfer_buffer;
            SDL_GPUBufferBinding* m_buffer_binding;
    };
}

#endif 