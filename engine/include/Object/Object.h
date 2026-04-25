#ifndef OBJECT_H
#define OBJECT_H

#include "Renderer/VBO.h"

namespace GCGameEngine{

    struct Mesh{
        VBO* vbo;
    };

    struct Transform{
        float x;
        float y;
        float z;
    };

    class Object{
        public:
            Object();
            ~Object();

            Mesh getMesh();
        private:
            Mesh mesh;
    };
}

#endif 