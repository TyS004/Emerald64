#ifndef UI_INPUT_H
#define UI_INPUT_H

#include "EditorCamera/EditorCamera.h"

namespace Editor{
    class EditorInput{
        public:
            static void Init();

            static void OnKeyPressed(SDL_Scancode scancode);
            static void OnKeyDown(SDL_Scancode scancode);
            static void OnMouseMove(SDL_MouseMotionEvent e);
            static void OnWindowResize(float width, float height);
            static void OnFileDropped(const char* path);

            static EditorCamera* getCamera();

            static bool debug_mode;
        private:
            static EditorCamera* camera;
    };
}

#endif