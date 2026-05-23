#ifndef ENGINE
#define ENGINE

#include "Renderer/VBO.h"
#include "Renderer/IBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"
#include "Layer/Layer.h"
#include "ECS/Entity.h"
#include "ECS/ComponetManager.h"
#include "ECS/Scene.h"
#include "Layer/SceneLayer.h"
#include "Serialization/SceneSerializer.h"
#include "ctx.h"

namespace E64{
    class Engine{
    public:
        static void run();
        static void exit();

        static std::unique_ptr<EngineCtx> ctx;
    private:
        static bool running;
    };
}

#endif