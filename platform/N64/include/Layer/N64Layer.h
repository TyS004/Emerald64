#ifndef N64LAYER_H
#define N64LAYER_H

#include <E64.h>
#include <t3d/t3d.h>
#include "Camera/N64Camera.h"

namespace E64{
    class N64Layer : public Layer{
        public:
            N64Layer();
            ~N64Layer();

            void OnUpdate(float dt);
            void OnEvent() {};
            void OnImGuiRender() {};
            void OnRender();
            void OnAttach(){};
        private:
            E64::Scene* scene;

            fm_mat4_t modelMat;
            T3DMat4FP* modelMatFP;

            N64Camera* camera;
    };
}

#endif
