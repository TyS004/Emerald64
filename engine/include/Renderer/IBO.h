#ifndef IBO_H
#define IBO_H

#include "SDL3/SDL.h"
#include "Window/Window.h"

namespace E64{
    class IBO{
        public:
            IBO();
            ~IBO();

            void upload(std::vector<uint32_t> indicies);

            SDL_GPUBuffer* getIndexBuffer();
        private:
            SDL_GPUBuffer* index_buffer;
    };
}

#endif 