#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include <json.hpp>
#include "Engine.h"
#include "ECS/ComponentManager.h"
#include "ECS/Component.h"

namespace E64{
    class SceneSerializer{
        public:
            SceneSerializer();
            ~SceneSerializer();

            void serialize();
            void deserialize();
    };
}

#endif