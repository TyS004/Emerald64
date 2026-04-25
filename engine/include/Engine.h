#ifndef ENGINE
#define ENGINE

#include "Renderer/VBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"
#include "Layer/Layer.h"
#include "Object/Object.h"

namespace GCGameEngine{
    class Engine{
    public:
        static void run();

        static void pushLayer(Layer* layer);
    private:
        static bool running;
        static std::vector<Layer*> layers;
    };
}

#endif