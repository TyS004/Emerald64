#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include <json.hpp>
#include "Engine.h"
#include "ECS/ComponetManager.h"
#include "ECS/Componet.h"

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