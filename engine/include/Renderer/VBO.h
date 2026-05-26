#ifndef VBO_H
#define VBO_H

#include "SDL3/SDL.h"

namespace E64{
    struct Vertex{
        glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 uv;
    };

    class VBO{
        public:
            VBO();
            VBO(std::vector<Vertex> vertices);
            ~VBO();

            void upload();
            SDL_GPUBufferBinding* getBufferBinding();

            int getNumVertices();
        private:
            void setTriangle();

            uint16_t num_vertices;
            std::vector<Vertex> vertices;

            SDL_GPUDevice* device;
            SDL_GPUBuffer* vertex_buffer;
            SDL_GPUTransferBuffer* transfer_buffer;
            SDL_GPUBufferBinding buffer_binding;
    };
}

#endif 