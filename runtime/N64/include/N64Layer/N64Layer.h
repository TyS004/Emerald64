#ifndef N64LAYER_H
#define N64LAYER_H

#include <E64.h>
namespace N64{
    class N64Layer : public E64::Layer{
        public:
            N64Layer();
            ~N64Layer();

            void OnAttach()      {};
            void OnUpdate()      {};
            void OnRender()      {};
            void OnImGuiRender() {};
    };
}

#endif