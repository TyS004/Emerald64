#include <E64.h>
#include <SDL3/SDL.h>

#include "Layer/UILayer.h"

int main(){
    std::cout << "Starting Editor" << std::endl;
    
    E64::Window::Create("Editor", 1300, 800);

    Editor::UILayer ui_layer = Editor::UILayer();
    E64::SceneLayer scene_layer = E64::SceneLayer();
    E64::Layer::Attach(&ui_layer);
    E64::Layer::Attach(&scene_layer);
    
    E64::Engine::run();

    E64::Window::Destory();
}