#include "Logger/Logger.h"

GCGameEngine::Log::Log(){}

void GCGameEngine::Log::info(const char* msg){
    std::cout << "[INFO]: " << msg << std::endl;
}

void GCGameEngine::Log::warn(const char* msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void GCGameEngine::Log::error(const char* msg){
    std::cout << "[ERROR]: " << msg << std::endl;
}

void GCGameEngine::Log::debug(const char* msg){
    std::cout << "[DEBUG]: " << msg << std::endl;
}

void GCGameEngine::Log::trace(const char* msg){
    std::cout << "[TRACE]: " << msg << std::endl;
} 