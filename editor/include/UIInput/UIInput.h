#ifndef UI_INPUT_H
#define UI_INPUT_H

#include "EditorCamera/EditorCamera.h"

namespace Editor{
    class UIInput{
        public:
            static void OnKeyPressed(SDL_Scancode scancode);
            static void OnKeyDown(SDL_Scancode scancode);
            static void OnMouseMove(SDL_MouseMotionEvent e);
            static void OnWindowResize(float width, float height);

            static EditorCamera* getCamera();

        private:
            static EditorCamera* camera;
    };
}

#endif