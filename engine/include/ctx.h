#ifndef CTX_H
#define CTX_H

#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Window/Window.h"
#include "ECS/Scene.h"
#include "AssetManager/AssetManager.h"

namespace E64{
    enum E64_MODE{
        EDITOR,
        DESKTOP_RUNTIME,
        N64_RUNTIME,
        NONE
    };

    struct EngineCtx{
        Window* window;
        IRenderer* renderer;
        Input* input;
        std::unique_ptr<E64::Scene> active_scene;
        std::unique_ptr<E64::AssetManager> asset_manager;

        E64_MODE mode = NONE;
    };
}

#endif