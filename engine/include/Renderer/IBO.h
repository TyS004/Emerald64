#ifndef IBO_H
#define IBO_H

#include "SDL3/SDL.h"
#include "Window/Window.h"

namespace GCGameEngine{
    class IBO{
        public:
            IBO();
            IBO(uint32_t* indicies, int num_indices);
            ~IBO();

            void sendToGPU();

            SDL_GPUBuffer* getIndexBuffer();

            int getNumIndices();
        private:
            int num_indices;
            uint32_t* indices;

            SDL_GPUBuffer* index_buffer;
    };
}

#endif 