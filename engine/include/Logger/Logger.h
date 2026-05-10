#ifndef LOGGER
#define LOGGER

#include <SDL3/SDL.h>

namespace E64{
    class Log
    {
        public:
            Log();

            static void info(const std::string& msg);
            static void warn(const std::string& msg);
            static void error(const std::string& msg);
            static void debug(const std::string& msg);
            static void trace(const std::string& msg);
    };
}

#endif