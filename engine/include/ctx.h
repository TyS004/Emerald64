#ifndef CTX_H
#define CTX_H

#include "Renderer/IRenderer.h"
#include "ECS/Scene.h"
#include "AssetManager/AssetManager.h"

namespace E64{
    struct EngineCtx{
        IRenderer* renderer;
        std::unique_ptr<E64::Scene> active_scene;
        std::unique_ptr<E64::AssetManager> asset_manager;
        bool Editor = false;
    };
}

#endif