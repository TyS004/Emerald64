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
            VBO(SDL_GPUDevice* device);
            ~VBO();
        private:
            Vertex* vertices;
            SDL_GPUBuffer* vertexBuffer;
            SDL_GPUTransferBuffer* transferBuffer;
            SDL_GPUDevice* device;
    };
}

#endif 