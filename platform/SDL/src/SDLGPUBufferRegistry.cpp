#include "SDLGPURegistry.h"

E64::GPUBufferHandle SDLGPURegistry::vbo_handle = 0;
std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> SDLGPURegistry::vbo_registry;

E64::GPUBufferHandle SDLGPURegistry::ibo_handle = 0;
std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> SDLGPURegistry::ibo_registry;

E64::GPUTextureHandle SDLGPURegistry::texture_handle = 0;
std::unordered_map<E64::GPUTextureHandle, SDL_GPUTexture*> SDLGPURegistry::texture_registry;

E64::GPUSamplerHandle SDLGPURegistry::sampler_handle = 0;
std::unordered_map<E64::GPUSamplerHandle, SDL_GPUSampler*> SDLGPURegistry::sampler_registry;