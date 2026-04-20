#ifndef PIPELINE
#define PIPELINE

#include "SDL3/SDL.h"
#include "Renderer/Window.h"
#include "Renderer/Shader.h"

namespace GCGameEngine{
    class Pipeline{
        public:
            Pipeline() {}
            Pipeline(SDL_GPUDevice* device, const char* shaderPath);

            ~Pipeline();
        private:
            SDL_GPUDevice* device;
            Shader* vertShader;
            Shader* fragShader;
            SDL_GPUGraphicsPipeline* pipeline;
    };
}
#endif