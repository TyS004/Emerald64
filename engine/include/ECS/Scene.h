#ifndef SCENE_H
#define SCENE_H

#include "ECS/Entity.h"
#include "Camera/Camera.h"

namespace GCGameEngine{
    class Scene{
        public:
            Scene();
            ~Scene();

            void pushEntity(ECS::Entity* entity);
            std::vector<ECS::Entity> getEntites();

            glm::mat4 getMVP();
            Camera* getCamera();
            
            void OnUpdate();
            void PrintScene();
        private:
            std::vector<ECS::Entity> entites;

            Camera* camera;
    };
}

#endif 