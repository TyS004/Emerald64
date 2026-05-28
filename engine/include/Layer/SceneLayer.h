#ifndef SCENE_LAYER_H
#define SCENE_LAYER_H

#include "Layer/Layer.h"
#include "E64API.h"

namespace E64{
    class E64_API SceneLayer : public Layer{
        public:
            SceneLayer();
            ~SceneLayer();

            void OnUpdate(float dt) {};
            void OnImGuiRender() {};
            void OnEvent(SDL_Event e) {};
            void OnRender();
            void OnAttach() {};

            LayerType GetLayerType() { return SCENE_LAYER; }
        private:
            Scene* scene;
    };
}

#endif