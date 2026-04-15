#include "log/logger.h"

Engine::Log::Log(){}

void Engine::Log::info(char* msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void Engine::Log::warn(char* msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void Engine::Log::debug(char* msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void Engine::Log::trace(char* msg){
    std::cout << "[WARN]: " << msg << std::endl;
}