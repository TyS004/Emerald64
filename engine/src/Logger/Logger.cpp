#include "Logger/Logger.h"

E64::Log::Log(){}

void E64::Log::info(const std::string& msg){
    std::cout << "[INFO]: " << msg << std::endl;
}

void E64::Log::warn(const std::string& msg){
    std::cout << "[WARN]: " << msg << std::endl;
}

void E64::Log::error(const std::string& msg){
    std::cout << "[ERROR]: " << msg << std::endl;
}

void E64::Log::debug(const std::string& msg){
    std::cout << "[DEBUG]: " << msg << std::endl;
}

void E64::Log::trace(const std::string& msg){
    std::cout << "[TRACE]: " << msg << std::endl;
} 