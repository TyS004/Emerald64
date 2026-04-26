#ifndef OBJECT_H
#define OBJECT_H

#include "Renderer/VBO.h"
#include "ECS/Componet.h"

namespace GCGameEngine{
    class Object{
        public:
            Object();
            ~Object();

            Componet::Mesh getMesh();
            glm::mat4 getModel();
        private:
            Componet::Mesh mesh;
            Componet::Transform transform;
            
            glm::mat4 model;
    };
}

#endif 