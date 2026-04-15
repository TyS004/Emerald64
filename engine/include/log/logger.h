#include <SDL3/SDL.h>

namespace Engine{
    class Log{
    public:
        Log();

        void info(char* msg);
        void warn(char* msg);
        void debug(char* msg);
        void trace(char* msg);
        
        void log(char* msg);
    };
}