#include <SDL3/SDL.h>
#include <GCGameEngine.h>

int main(){
    GCGameEngine::Window::Create("Editor", 800, 600);

    std::cout << "Starting Editor" << std::endl;
    GCGameEngine::Engine::run();

    GCGameEngine::Window::Destory();
}