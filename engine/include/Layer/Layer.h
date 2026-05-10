#ifndef LAYER_H
#define LAYER_H

#include "ctx.h"

namespace E64{
    class Layer{
        public: 
            enum LayerType{
                SCENE_LAYER,
                UI_LAYER,
                LAYER
            };
            virtual ~Layer() = default;

            static void Attach(Layer* layer) { layers.push_back(layer); };

            virtual void OnUpdate() {};
            virtual void OnEvent(SDL_Event* e) {};
            virtual void OnImGuiRender(E64::EngineCtx* ctx) {};
            virtual void OnRender() {};

            virtual LayerType GetLayerType() { return LAYER; };

            inline static std::vector<Layer*> layers = {};
    };
}

#endif