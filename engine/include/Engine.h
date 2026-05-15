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
#include "Renderer/FBXParser.h"
#include "Layer/EditorLayer.h"
#include "Layer/SceneLayer.h"
#include "ctx.h"

namespace E64{
    class Engine{
    public:
        static void run();
        static void exit();

        static EngineCtx* ctx;
    private:
        static bool running;
    };
}

#endif