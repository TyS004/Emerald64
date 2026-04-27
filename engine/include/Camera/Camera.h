#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "ECS/Componet.h"
#include "ECS/Entity.h"

namespace GCGameEngine{
    class Camera{
        public:
            Camera();
            ~Camera();

            void OnUpdate();

            ECS::Transform getTransform();
            void setTransform(ECS::Transform transform);

            glm::mat4 getView();
            glm::mat4 getProj();
        private:
            glm::mat4 view;
            glm::mat4 proj;

            ECS::Transform transform;
    };
}

#endif