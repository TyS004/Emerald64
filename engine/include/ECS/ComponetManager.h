#ifndef COMPONET_MANAGER_H
#define COMPONET_MANAGER_H

#include "ECS/Componet.h"
#include "ECS/Entity.h"
#include <vector>

namespace GCGameEngine{
    namespace ECS{
        class ComponetManager{
            public:
                template <typename T>
                static T getComponet(Entity e);

                template <typename T>
                static bool hasComponet(Entity e);
    
                template <typename T>
                static void addComponet(Entity e, T componet);

                static void clean();
    
            private:
                static std::vector<Transform*> transforms;
                static std::vector<Mesh*> meshes;
        };
    }
}

#endif