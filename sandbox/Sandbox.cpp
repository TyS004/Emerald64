#include "Sandbox.h"
#include <E64.h>

Sandbox::Sandbox(){
    
}

Sandbox* Sandbox::run(){
    std::cout << "Starting Engine" << std::endl;

    //E64::Engine::run();
    
    return new Sandbox;
}