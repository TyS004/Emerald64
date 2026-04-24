#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "SDL3/SDL.h"
#include "Logger/Logger.h"
#include "Event/Event.h"

namespace GCGameEngine{
    class EventDispatcher{
        public:
            static bool handle(SDL_Event event);
    };
}

#endif