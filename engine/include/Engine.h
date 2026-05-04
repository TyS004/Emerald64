#ifndef ENGINE
#define ENGINE

#include "Renderer/VBO.h"
#include "Renderer/IBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"
#include "Layer/Layer.h"
#include "ECS/Entity.h"
#include "ECS/ComponetManager.h"
#include "Camera/Camera.h"
#include "ECS/Scene.h"
#include "Renderer/FBXParser.h"

namespace GCGameEngine{
    class Engine{
    public:
        static void run();
        static void exit();

        static void pushLayer(Layer* layer);
        static void setActiveScene(Scene* scene);
    private:
        static bool running;
        static std::vector<Layer*> layers;
        static Scene* active_scene;
    };
}

#endif