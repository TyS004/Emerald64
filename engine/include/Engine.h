#ifndef ENGINE
#define ENGINE

#include "Renderer/VBO.h"
#include "Renderer/Renderer.h"
#include "Renderer/Pipeline.h"
#include "Event/EventDispatcher.h"
#include "SDL3/SDL.h"

namespace GCGameEngine{
    class Engine{
    public:
        static void run();
    private:
        static bool running;
    };
}

#endif