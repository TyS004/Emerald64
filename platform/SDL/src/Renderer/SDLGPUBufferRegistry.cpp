#include "Renderer/SDLGPURegistry.h"

E64::GPUBufferHandle E64::SDLGPURegistry::vbo_handle = 0;
std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> E64::SDLGPURegistry::vbo_registry;

E64::GPUBufferHandle E64::SDLGPURegistry::ibo_handle = 0;
std::unordered_map<E64::GPUBufferHandle, SDL_GPUBuffer*> E64::SDLGPURegistry::ibo_registry;

E64::GPUTextureHandle E64::SDLGPURegistry::texture_handle = 0;
std::unordered_map<E64::GPUTextureHandle, SDL_GPUTexture*> E64::SDLGPURegistry::texture_registry;

E64::GPUSamplerHandle E64::SDLGPURegistry::sampler_handle = 0;
std::unordered_map<E64::GPUSamplerHandle, SDL_GPUSampler*> E64::SDLGPURegistry::sampler_registry;