#ifndef N64LAYER_H
#define N64LAYER_H

#include "Layer/Layer.h"

namespace E64{
    class N64Layer : public E64::Layer{
        public:
            N64Layer();
            ~N64Layer();

            void OnUpdate(float dt) {};
            void OnRender() {};
            void OnImGuiRender() {};
            void OnAttach() {};
            void OnEvent() {};
    };
}

#endif