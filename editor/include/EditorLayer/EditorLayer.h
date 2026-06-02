#ifndef EditorLayer_H
#define EditorLayer_H

#include <E64.h>
#include <Window/SDLWindow.h>

#include "EditorInput/EditorInput.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>
#include <misc/freetype/imgui_freetype.h>

namespace Editor{
    class EditorLayer : public E64::Layer{
        public:
            EditorLayer();
            ~EditorLayer();

            void OnUpdate(float dt);
            void OnEvent() {};
            void OnImGuiRender();
            void OnRender() {};
            void OnAttach();

            E64::Layer::LayerType GetLayerType() { return EDITOR_LAYER; }
        private:
            void initStyle();

            void buildDockspace();
            void buildMainMenuBar();

            void buildViewport();
            void buildDebug(ImVec2 viewport_tl);
            void buildSceneSelector();

            void buildInspector();
                void buildTransformHeader();
                void buildMeshHeader();
                void buildCameraHeader();

            void buildFileManager();

            int selected;
            float FPS;
            float ms;
            float debug_pad = 15.0f;

            ImVec2 viewport_size;
            ImFont* font;

            E64::SDLWindow* sdl_window;
            EditorInput* input;
            EditorCamera* camera;
    };
}

#endif