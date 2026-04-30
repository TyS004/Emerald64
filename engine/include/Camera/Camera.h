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

            ECS::Transform* getTransform();
            void setTransform(ECS::Transform* transform);

            glm::mat4 getView();
            glm::mat4 getProj();
            glm::vec3 getCameraFront();
            float getSens();
            float getVelocity();
        private:
            glm::mat4 view;
            glm::mat4 proj;
            
            glm::vec3 camera_front;

            ECS::Transform* transform;

            float cam_sens;
            float cam_velocity;
    };
}

#endif