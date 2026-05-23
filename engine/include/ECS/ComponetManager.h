#ifndef COMPONET_MANAGER_H
#define COMPONET_MANAGER_H

#include "ECS/Componet.h"
#include "ECS/Entity.h"
#include <vector>

namespace E64{
    namespace ECS{
        class ComponetManager{
            public:
                template <typename T>
                static T* getComponet(Entity e);

                template <typename T>
                static bool hasComponet(Entity e);
    
                template <typename T>
                static void addComponet(Entity e, T comp);

                static void flushComponets();

                static void clean();
    
            private:
                static std::vector<TransformComponet> transform_componets;
                static std::vector<MeshComponet> mesh_componets;
        };
    }
}

#endif