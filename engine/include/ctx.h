#ifndef CTX_H
#define CTX_H

#include "Renderer/Renderer.h"
#include "ECS/Scene.h"
#include "AssetManager/AssetManager.h"
#include <chrono>

namespace E64{
    struct EngineCtx{
        std::unique_ptr<E64::Renderer> renderer;
        std::unique_ptr<E64::Scene> active_scene;
        std::unique_ptr<E64::AssetManager> asset_manager;
        bool Editor = false;
    };
}

#endif