#include "EditorInput/EditorInput.h"
#include "AssetImporter/AssetImporter.h"
#include <E64.h>
#include <filesystem>

using namespace E64;

int mouse_offset_x = 0;
int mouse_offset_y = 0;

Editor::EditorCamera* Editor::EditorInput::camera = new Editor::EditorCamera();
bool Editor::EditorInput::debug_mode = true;
int Editor::EditorInput::selected_entity = 0;

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
            if(E64::Window::isMouseLock()){
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
        case SDL_SCANCODE_F12:
            std::system("./E64Runtime");
            break;
        default:
            break;
    }
}

void Editor::EditorInput::OnMouseMove(SDL_MouseMotionEvent e){
    // std::string msg =  "UI Layer Mouse Move Event: " + std::to_string(e.x) + ", " + std::to_string(e.y);
    // E64::Log::debug(msg);
}

void Editor::EditorInput::OnWindowResize(float width, float height){
    E64::Engine::ctx->renderer->OnImGuiResize(width, height);

    std::string msg =  "UI Layer Window Resize Event: " + std::to_string(width) + ", " + std::to_string(height);
    E64::Log::debug(msg);
}

void Editor::EditorInput::OnFileDropped(const char* path){
    std::filesystem::path fs_path = path;
    std::filesystem::path ext = std::filesystem::path(path).extension();
    E64::Log::debug(std::string(path));

    if(ext == ".obj"){
        AssetImporter importer;
        ECS::Mesh mesh = importer.importMesh(fs_path);
        ECS::MeshComponent comp = E64::Engine::ctx->asset_manager->addMesh(mesh);
        ECS::MeshComponent* mesh_comp = ECS::ComponentManager::getComponent<ECS::MeshComponent>(selected_entity);
        if(mesh_comp) mesh_comp->mesh_handle = comp.mesh_handle;
        else {
            E64::Log::error("Entity Has No Mesh Component! \nCreate a Mesh Component for this Entity to Assign a Mesh to it.");
        }
    }

    else if(ext == ".png" || ext == ".jpg" || ext == ".bmp"){
        ECS::MeshComponent* mesh_componet = ECS::ComponentManager::getComponent<ECS::MeshComponent>(selected_entity);
        ECS::Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_componet->mesh_handle);
        mesh->texture = TBO(fs_path);
        mesh->texture.upload();
    }
}

Editor::EditorCamera* Editor::EditorInput::getCamera(){
    return camera;
}