#ifndef SDLREGISTRY_H
#define SDLREGISTRY_H

#include <unordered_map>
#include <E64.h>
#include <SDL3/SDL.h>

namespace E64 {
    struct SDLGPURegistry{
        static E64::GPUBufferHandle vbo_handle;
        static std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> vbo_registry;

        static E64::GPUBufferHandle ibo_handle;
        static std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> ibo_registry;

        static E64::GPUTextureHandle texture_handle;
        static std::unordered_map<E64::GPUTextureHandle, SDL_GPUTexture*> texture_registry;
        
        static E64::GPUSamplerHandle sampler_handle;
        static std::unordered_map<E64::GPUSamplerHandle, SDL_GPUSampler*> sampler_registry;
    };
}

#endif