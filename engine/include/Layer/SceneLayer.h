#ifndef SCENE_LAYER_H
#define SCENE_LAYER_H

#include "Layer/Layer.h"

namespace E64{
    class SceneLayer : public Layer{
        public:
            SceneLayer();
            ~SceneLayer();

            void OnUpdate(float dt) {};
            void OnImGuiRender() {};
            void OnEvent() {};
            void OnRender();
            void OnAttach() {};

            LayerType GetLayerType() { return SCENE_LAYER; }
        private:
            Scene* scene;
    };
}

#endif