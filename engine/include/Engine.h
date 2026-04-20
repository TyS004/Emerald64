#ifndef ENGINE
#define ENGINE

#include "Renderer/Window.h"
#include "Renderer/VBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"

namespace GCGameEngine{
    class Engine{
    public:
        static void run();
    private:
        static bool isRunning;
    };
}

#endif