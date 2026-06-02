#ifndef WINDOW_H
#define WINDOW_H

namespace E64{
    class Window
    {
        public:
            virtual ~Window() = default;

            virtual void create(const char* name, int width, int height) = 0;

            virtual void setMouseLock(bool lock) = 0;
            virtual bool isMouseLock() = 0;

            virtual uint32_t getWidth() = 0;
            virtual uint32_t getHeight() = 0;

            virtual bool getVSync() = 0;
            virtual void toggleVSync() = 0;
    };
}

#endif