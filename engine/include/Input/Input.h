#pragma once

#include "Input/Scancode.h"

namespace E64 {
    class Input {
    public:
        virtual ~Input() = default;

        virtual void poll() = 0;

        virtual bool isRunning() { return true; };
        virtual bool isKeyPressed(Scancode key) { return false; };
        virtual bool isKeyDown(Scancode key) { return false; };

        virtual void OnKeyPressed(Scancode key) {}
        virtual void OnKeyDown(Scancode key) {};      
        virtual void OnMouseMove(float xrel, float yrel) {}
        virtual void OnWindowResize(int w, int h) {}

        virtual float getXRel() { return 0.0f;};
        virtual float getYRel() { return 0.0f; };

    protected:
        bool running = true;
        float xrel = 0.0f;
        float yrel = 0.0f;
    };
}