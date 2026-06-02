// EditorInput.h
#pragma once

#include "Input/SDLInput.h"
#include "EditorCamera/EditorCamera.h"

namespace Editor {
    class EditorInput : public E64::SDLInput {
    public:
        EditorInput();

        void OnKeyPressed(E64::Scancode key) override;
        void OnKeyDown(E64::Scancode key) override;
        void OnMouseMove(float xrel, float yrel) override;
        void OnWindowResize(int w, int h) override;
        void OnFileDropped(const char* path);

        bool debug_mode = true;
        int selected_entity = 0;

    protected:
        void processEvent(SDL_Event& e) override;
    };
}