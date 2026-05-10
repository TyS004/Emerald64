#ifndef CTX_H
#define CTX_H

#include "Renderer/Renderer.h"
#include "ECS/Scene.h"
#include <chrono>

namespace E64{
    struct EngineCtx{
        E64::Renderer* renderer;
        E64::Scene* active_scene;
        double elapsed;
    };
}


#endif