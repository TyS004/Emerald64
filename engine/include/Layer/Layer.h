#ifndef LAYER_H
#define LAYER_H

#include "ctx.h"
#include "E64Core.h"

namespace E64{
    class E64_API Layer{
        public: 
            enum LayerType{
                SCENE_LAYER,
                EDITOR_LAYER,
                RUNTIME_LAYER,
                LAYER
            };
            virtual ~Layer() = default;

            static void Attach(Layer* layer) { 
                layers.push_back(layer);
                layer->OnAttach();
                
                E64::Log::debug("Attached Layer");
            };

            virtual void OnUpdate(float dt) {};
            virtual void OnEvent() {};
            virtual void OnImGuiRender() {};
            virtual void OnRender() {};
            virtual void OnAttach() {};

            virtual LayerType GetLayerType() { return LAYER; };

            static std::vector<Layer*> layers;
    };
}

#endif