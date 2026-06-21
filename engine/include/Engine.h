#ifndef ENGINE_H
#define ENGINE_H

#include "ctx.h"

namespace E64{
    class E64_API Engine{
    public:
        static void run();
        static void exit();

        static std::unique_ptr<EngineCtx> ctx;
    private:
        static bool running;
    };
}

#endif