#ifndef IBO_H
#define IBO_H

#include "SDL3/SDL.h"
#include "Window/Window.h"

namespace E64{
    class IBO{
        public:
            IBO();
            IBO(std::vector<uint32_t> indicies);
            ~IBO();

            void upload();

            SDL_GPUBuffer* getIndexBuffer();

            int getNumIndices();
        private:
            int num_indices;
            std::vector<uint32_t> indices;

            SDL_GPUBuffer* index_buffer;
    };
}

#endif 