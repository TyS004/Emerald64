#ifndef EDITOR_INPUT_H
#define EDITOR_INPUT_H

namespace Editor{
    class EditorInput{
        public:
            static void OnKeyPressed(SDL_Scancode scancode);
            static void OnKeyDown(SDL_Scancode scancode);
            static void OnMouseMove(SDL_MouseMotionEvent e);
    };
}

#endif