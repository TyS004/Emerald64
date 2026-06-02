// Input/SDLInput.h — SDL lives here only
#pragma once

#include "Input/Input.h"
#include <SDL3/SDL.h>

namespace E64{
    class SDLInput : public Input {
    public:
        SDLInput();

        void poll() override;

        bool isRunning() override { return running; }
        bool isKeyPressed(Scancode key) override;
        bool isKeyDown(Scancode key) override;

        virtual void OnKeyPressed(Scancode key) override;

        float getXRel() override { float x = xrel; xrel = 0.0f; return x; }
        float getYRel() override { float y = yrel; yrel = 0.0f; return y; }

        virtual void processEvent(SDL_Event& e);

    private:
        E64::Scancode key_map[SDL_SCANCODE_COUNT] = {};
        
        void buildKeyMap();
    };
}
