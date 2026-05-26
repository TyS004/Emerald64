#ifndef LAYER_H
#define LAYER_H

#include "ctx.h"

namespace E64{
    class Layer{
        public: 
            enum LayerType{
                SCENE_LAYER,
                EDITOR_LAYER,
                LAYER
            };
            virtual ~Layer() = default;

            static void Attach(Layer* layer) { layers.push_back(layer); layer->OnAttach(); };

            virtual void OnUpdate(float dt) {};
            virtual void OnEvent(SDL_Event* e) {};
            virtual void OnImGuiRender() {};
            virtual void OnRender() {};
            virtual void OnAttach() {};

            virtual LayerType GetLayerType() { return LAYER; };

            inline static std::vector<Layer*> layers = {};
    };
}

#endif