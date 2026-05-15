#ifndef EditorLayer_H
#define EditorLayer_H

#include <E64.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

namespace Editor{
    class EditorLayer : public E64::Layer{
        public:
            EditorLayer();
            ~EditorLayer();

            void OnUpdate(float dt);
            void OnEvent(SDL_Event* e);
            void OnImGuiRender();
            void OnRender() {};

            E64::Layer::LayerType GetLayerType() { return UI_LAYER; }
        private:
            ImVec2 viewport_size;

            void buildDockspace();
            void buildViewport();
            void buildSceneSelector();

            void buildInspector();
            void buildTransformHeader();
            void buildMeshHeader();

            void buildFileManager();

            int selected;

            void buildDebug(ImVec2 viewport_tl);

            float FPS;
            float ms;

            float debug_pad = 15.0f;
    };
}

#endif