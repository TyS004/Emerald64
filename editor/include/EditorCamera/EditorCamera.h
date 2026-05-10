#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ECS/Componet.h"
#include "ECS/Entity.h"

namespace Editor{
    class EditorCamera{
        public:
            EditorCamera();
            ~EditorCamera();

            void OnUpdate();
            void OnResize(float width, float height);

            E64::ECS::Transform* getTransform();
            void setTransform(E64::ECS::Transform* transform);

            glm::mat4 getView();
            glm::mat4 getProj();
            glm::vec3 getCameraFront();
            float getSens();
            float getVelocity();
        private:
            glm::mat4 view;
            glm::mat4 proj;
            
            glm::vec3 camera_front;

            E64::ECS::Transform* transform;

            float cam_sens;
            float cam_velocity;
    };
}

#endif