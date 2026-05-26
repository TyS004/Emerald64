#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>
#include "Window/Window.h"
#include "Utils/stb_image.h"

namespace E64{
    class Texture{
        public:
            Texture();
            Texture(std::string path);
            Texture(const char* path);
            ~Texture();

            void upload();

            SDL_GPUTexture* getTexture();
            SDL_GPUSampler* getSampler();
        private:
            SDL_GPUTexture* texture;
            SDL_GPUSampler* sampler;

            int width;
            int height;
            int channels;
            int img_size;
            unsigned char* img_data;

            void sendToGPU();
    };
}

#endif