#include "Event/EventDispatcher.h"

static GCGameEngine::Log logger;

void onKeyboardPressed(){
    logger.debug("Keyboard Pressed");
}

bool GCGameEngine::EventDispatcher::handle(SDL_Event event){
    switch(event.type){
        case SDL_EVENT_KEY_DOWN:
            onKeyboardPressed();
            break;
        case SDL_EVENT_QUIT:
            return false;
    }
    return true;
}