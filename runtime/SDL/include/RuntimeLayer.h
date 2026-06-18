#ifndef RUNTIME_LAYER_H
#define RUNTIME_LAYER_H

#include <E64.h>

namespace Runtime{
    class RuntimeLayer : public E64::Layer{
        public:
            RuntimeLayer();
            ~RuntimeLayer();

            void OnUpdate(float dt);
            void OnEvent() {};
            void OnImGuiRender() {};
            void OnRender() {};
            void OnAttach() {};

            E64::Layer::LayerType GetLayerType() { return RUNTIME_LAYER; }
        private:
            float angle;
    };
}

#endif