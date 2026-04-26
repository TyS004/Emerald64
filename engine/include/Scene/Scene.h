#ifndef SCENE_H
#define SCENE_H

#include "ECS/Entity.h"
#include "Camera/Camera.h"
#include "Object/Object.h"

namespace GCGameEngine{
    class Scene{
        public:
            Scene();
            ~Scene();

            void pushEntity(Entity* entity);
            glm::mat4 getMVP();

            Object* getObject();
        private:
            Entity** entites;
            glm::mat4 mvp;

            //TEMP
            Object* obj;
    };
}

#endif 