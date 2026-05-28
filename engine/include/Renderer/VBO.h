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
            ~VBO();

            void upload(std::vector<Vertex> vertices);

            SDL_GPUBuffer* getVertexBuffer();
        private:
            SDL_GPUBuffer* vertex_buffer;
    };
}

#endif 