#ifndef PIPELINE
#define PIPELINE

#include "SDL3/SDL.h"
#include "Window/Window.h"
#include "Renderer/Shader.h"

namespace E64{
    class Pipeline{
        public:
            Pipeline() {}
            Pipeline(const char* shader_path);

            ~Pipeline();

            SDL_GPUGraphicsPipeline* getPipeline();
        private:
            SDL_GPUDevice* device;
            Shader* vert_shader;
            Shader* frag_shader;

            SDL_GPUGraphicsPipeline* pipeline;
    };
}
#endif