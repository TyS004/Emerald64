#ifndef N64WINDOW_H
#define N64WINDOW_H

#include <E64.h>
#include <libdragon.h>
namespace E64{
    enum N64DisplayMode{
        CONSOLE,
        RDP
    };

    class N64Window : public Window{
        public:
        N64Window() {};
        N64Window(N64DisplayMode mode, int16_t FB_COUNT);
        ~N64Window();

        void create(const char* name, int width, int height) {};
        void destory() {};

        void setMouseLock(bool lock) {  };
        bool isMouseLock() { return false; };

        bool getVSync() { return false; };
        void toggleVSync() {  };

        uint32_t getWidth() { return width; };
        uint32_t  getHeight() { return height; };

        void toggleDisplayMode();

        N64DisplayMode* getMode();
        void setMode(N64DisplayMode mode);

        private:
            N64DisplayMode mode;

            uint32_t width = 640;
            uint32_t height = 480;

            int16_t FB_COUNT;
    };
}

#endif