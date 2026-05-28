#include <E64.h>

int main(){
    E64::Window::Create("E64 Runtime", 800, 600);
    E64::SceneLayer* runtime_layer = new E64::SceneLayer();
    E64::Layer::Attach(runtime_layer);
    E64::Engine::run();
}