#ifndef ENGINE
#define ENGINE

#include "Renderer/VBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"
#include "Layer/Layer.h"
#include "Object/Object.h"
#include "ECS/Entity.h"
#include "ECS/ComponetManager.h"
#include "Camera/Camera.h"
#include "Scene/Scene.h"

namespace GCGameEngine{
    class Engine{
    public:
        static void run();

        static void pushLayer(Layer* layer);
        static void setActiveScene(Scene* scene);
    private:
        static bool running;
        static std::vector<Layer*> layers;
        static Scene* active_scene;
    };
}

#endif