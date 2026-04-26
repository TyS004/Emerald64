#ifndef OBJECT_H
#define OBJECT_H

#include "Renderer/VBO.h"

namespace GCGameEngine{

    struct Transform{
        glm::vec3 transform;
    };

    struct Mesh{
        VBO* vbo;
    };

    class Object{
        public:
            Object();
            ~Object();

            Mesh getMesh();
            glm::mat4 getMVP();
        private:
            Mesh mesh;
            Transform transform;
            glm::mat4 mvp;
    };
}

#endif 