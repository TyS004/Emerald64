// engine/include/E64API.h
#pragma once

#ifdef _WIN32
    #ifdef E64_EXPORTS
        #define E64_API __declspec(dllexport)
    #else
        #define E64_API __declspec(dllimport)
    #endif
#else
    #define E64_API
#endif