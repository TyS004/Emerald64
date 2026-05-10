#ifndef UILAYER_H
#define UILAYER_H

#include <E64.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Editor{
    class UILayer : public E64::Layer{
        public:
            UILayer();
            ~UILayer();

            void OnUpdate();
            void OnEvent(SDL_Event* e);
            void OnImGuiRender(E64::EngineCtx* ctx);
            void OnRender() {};

            E64::Layer::LayerType GetLayerType() { return UI_LAYER; }
        private:
            ImVec2 viewport_size;
            E64::Scene* scene;
    };
}

#endif