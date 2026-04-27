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

            glm::mat4 getMVP();
            Camera* getCamera();

            std::vector<ECS::Entity> getEntites();
            
            void OnUpdate();
        private:
            std::vector<ECS::Entity> entites;

            Camera* camera;
    };
}

#endif 