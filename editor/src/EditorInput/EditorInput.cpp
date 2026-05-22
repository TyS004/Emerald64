#include "EditorInput/EditorInput.h"
#include <E64.h>
#include "FBXParser/FBXParser.h"

using namespace E64;

int mouse_offset_x = 0;
int mouse_offset_y = 0;

Editor::EditorCamera* Editor::EditorInput::camera = new Editor::EditorCamera();
bool Editor::EditorInput::debug_mode = true;

void Editor::EditorInput::Init(){
    E64::Input::OnKeyPressedBind  = Editor::EditorInput::OnKeyPressed;
    E64::Input::OnKeyDownBind     = Editor::EditorInput::OnKeyDown;
    E64::Input::OnMouseMoveBind   = Editor::EditorInput::OnMouseMove;
    E64::Input::OnFileDroppedBind = Editor::EditorInput::OnFileDropped;
}

void Editor::EditorInput::OnKeyPressed(SDL_Scancode scancode){
    if(E64::Input::isKeyPressed(SDL_SCANCODE_ESCAPE)) E64::Engine::exit();
}

void Editor::EditorInput::OnKeyDown(SDL_Scancode scancode){
    switch(scancode){
        case SDL_SCANCODE_Z:
            if(E64::Window::isMouseLock())
            {
                E64::Window::setMouseLock(false);
            }
            else{
                SDL_HideCursor();
                E64::Window::setMouseLock(true);
            }
            break;
        case SDL_SCANCODE_V:
            E64::Window::toggleVSync();
            break;
        case SDL_SCANCODE_F1:
            debug_mode = !debug_mode;
            break;
        default:
            break;
    }
}

void Editor::EditorInput::OnMouseMove(SDL_MouseMotionEvent e){
    E64::ECS::TransformComponet* transform = camera->getTransform();
    
    // std::string msg =  "UI Layer Mouse Move Event: " + std::to_string(e.x) + ", " + std::to_string(e.y);
    // E64::Log::debug(msg);
}

void Editor::EditorInput::OnWindowResize(float width, float height){
    E64::Engine::ctx->renderer->OnResize(width, height);

    std::string msg =  "UI Layer Window Resize Event: " + std::to_string(width) + ", " + std::to_string(height);
    E64::Log::debug(msg);
}

void Editor::EditorInput::OnFileDropped(const char* path){
    FBXParser parser = FBXParser();
    ECS::Mesh* dropped_mesh = parser.getMesh(path);

    uint32_t id = E64::Engine::ctx->asset_manager->addMesh(dropped_mesh);

    for(ECS::Entity e : E64::Engine::ctx->active_scene->getEntites()){
        ECS::MeshComponet* mesh_componet = ECS::ComponetManager::getComponet<ECS::MeshComponet>(e);
        mesh_componet->id = id;
    }
    
    delete dropped_mesh;
}

Editor::EditorCamera* Editor::EditorInput::getCamera(){
    return camera;
}