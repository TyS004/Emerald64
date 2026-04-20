#ifndef LOGGER
#define LOGGER

#include <SDL3/SDL.h>

namespace GCGameEngine{
    class Log
    {
        public:
            Log();

            static void info(const char* msg);
            static void warn(const char* msg);
            static void error(const char* msg);
            static void debug(const char* msg);
            static void trace(const char* msg);
            
            static void log(const char* msg);
    };
}

#endif