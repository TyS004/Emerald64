#ifndef SCENE_H
#define SCENE_H

#include "ECS/Entity.h"

namespace E64{
    struct CameraData{
        glm::mat4 proj;
        glm::mat4 view;
    };

    class Scene{
        public:
            Scene();
            Scene(Scene&& other) noexcept;
            ~Scene();

            void pushEntity(ECS::Entity entity);
            std::vector<ECS::Entity> getEntites();

            std::string getName();
            glm::mat4 getMVP();
            CameraData getCameraData();
            void setCameraData(CameraData camera_data);
            
            void printScene();
            void render();
        private:
            std::string name;

            std::vector<ECS::Entity> entites;
            CameraData camera_data;
    };
}

#endif 