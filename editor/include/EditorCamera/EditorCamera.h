#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include <E64.h>
#include <Input/SDLInput.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Editor{
    class EditorCamera{
        public:
            EditorCamera();
            ~EditorCamera();

            void OnUpdate(float dt);
            void OnResize(float width, float height);

            E64::ECS::TransformComponent* getTransform();
            void setTransform(E64::ECS::TransformComponent* transform);

            glm::mat4 getView();
            glm::mat4 getProj();
            glm::vec3 getCameraFront();
            float getSens();
            float getVelocity();
        private:
            E64::SDLInput* input;
        
            glm::mat4 view;
            glm::mat4 proj;
            
            glm::vec3 camera_front;

            E64::ECS::TransformComponent* transform;

            float cam_sens;
            float cam_vel;
            float xrel, yrel;
            float prev_xrel, prev_yrel;
    };
}

#endif