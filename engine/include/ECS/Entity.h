#ifndef ENTITY_H
#define ENTITY_H

#include <uuid/uuid.h>
#include <unordered_map>

#include "ECS/Componet.h"

namespace E64{
    namespace ECS{
        using Entity = uint32_t;

        class EntityManager{
            public:
                static Entity createEntity();
                static void deleteEntity();

                static std::unordered_map<Entity, ComponetMask> entity_index;
        };
    }
}

#endif