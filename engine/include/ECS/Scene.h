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
            void setCameraData(ECS::CameraData camera);

            ECS::Entity getCamera();
            void setCamera(ECS::Entity camera);
            
            void printScene();

        private:
            std::string name;
            ECS::CameraData active_camera_data;
            ECS::Entity camera;
            std::vector<ECS::Entity> entites;

            bool editorRender;
    };
}

#endif 