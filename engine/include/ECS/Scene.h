#ifndef SCENE_H
#define SCENE_H

#include "ECS/Entity.h"

namespace E64{
    class Scene{
        public:
            Scene();
            Scene(Scene&& other) noexcept;
            ~Scene();

            void createDefaultScene();

            void pushEntity();
            void pushEntity(ECS::Entity e);
            std::vector<ECS::Entity> getEntites();

            std::string getName();
            glm::mat4 getMVP();
            ECS::CameraData getCameraData();
            void setCamera(ECS::Entity camera);
            void setCameraData(ECS::CameraData camera);
            
            void printScene();
            void render();
        private:
            std::string name;
            ECS::Entity runtime_camera;
            ECS::CameraData camera_data;
            std::vector<ECS::Entity> entites;
    };
}

#endif 