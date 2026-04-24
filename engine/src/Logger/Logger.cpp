#include "Logger/Logger.h"

GCGameEngine::Log::Log(){}

void GCGameEngine::Log::info(const std::string& msg){
    std::cout << "[INFO]: " << msg << std::endl;
}

void GCGameEngine::Log::warn(const std::string& msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void GCGameEngine::Log::error(const std::string& msg){
    std::cout << "[ERROR]: " << msg << std::endl;
}

void GCGameEngine::Log::debug(const std::string& msg){
    std::cout << "[DEBUG]: " << msg << std::endl;
}

void GCGameEngine::Log::trace(const std::string& msg){
    std::cout << "[TRACE]: " << msg << std::endl;
} 