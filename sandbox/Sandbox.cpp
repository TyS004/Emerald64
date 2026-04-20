#include "Sandbox.h"
#include <GCGameEngine.h>

Sandbox::Sandbox(){
    
}

Sandbox* Sandbox::run(){
    std::cout << "Starting Engine" << std::endl;

    GCGameEngine::Engine::run();
    
    return new Sandbox;
}