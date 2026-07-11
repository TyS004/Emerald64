#ifndef PHYSICS_LAYER_H
#define PHYSICS_LAYER_H

#include "Layer/Layer.h"

namespace E64{
    class PhysicsLayer : public Layer {
        public:
            PhysicsLayer();
            ~PhysicsLayer();

            void OnAttach() {};
            void OnUpdate(float dt);
            void OnRender() {};
            void OnImGuiRender() {};

            void CameraUpdate();

            static bool running;
            
        private:
            float velocity;
            float acceleration;

            E64::Scene* scene;
    };
}

#endif