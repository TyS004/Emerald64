#ifndef OBJECT_H
#define OBJECT_H

#include "Renderer/VBO.h"
#include "ECS/Componet.h"
#include "Camera/Camera.h"

namespace GCGameEngine{
    class Object{
        public:
            Object();
            ~Object();

            glm::mat4 getModel();

            void setMVP(glm::mat4 mvp);
            glm::mat4 getMVP();

            void OnUpdate(Camera* camera);

            ECS::Mesh mesh;
            ECS::Transform transform;
        private:
            glm::mat4 model;
            glm::mat4 mvp;
    };
}

#endif 