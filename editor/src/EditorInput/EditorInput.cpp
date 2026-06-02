// EditorInput.cpp
#include "EditorInput/EditorInput.h"
#include "AssetImporter/AssetImporter.h"

#include <E64.h>
#include <filesystem>

Editor::EditorInput::EditorInput() : E64::SDLInput() {

}

void Editor::EditorInput::processEvent(SDL_Event& e) {
    E64::SDLInput::processEvent(e);
    if (e.type == SDL_EVENT_DROP_FILE)
        OnFileDropped(e.drop.data);
}

void Editor::EditorInput::OnKeyPressed(E64::Scancode key) {
    if (key == E64::Scancode::Escape) E64::Engine::exit();

    switch (key) {
        case E64::Scancode::Z:
            if (E64::Window::isMouseLock()) E64::Window::setMouseLock(false);
            else { SDL_HideCursor(); E64::Window::setMouseLock(true); }
            break;
        case E64::Scancode::V:
            E64::Window::toggleVSync();
            break;
        case E64::Scancode::F1:
            debug_mode = !debug_mode;
            break;
        case E64::Scancode::F12:
            std::system("./E64Runtime");
            break;
        default:
            break;
    }
}

void Editor::EditorInput::OnKeyDown(E64::Scancode key) {

}

void Editor::EditorInput::OnMouseMove(float xrel, float yrel) {
}

void Editor::EditorInput::OnWindowResize(int w, int h) {
    E64::Engine::ctx->renderer->OnImGuiResize(w, h);
    E64::Log::debug("UI Layer Window Resize Event: " + std::to_string(w) + ", " + std::to_string(h));
}

void Editor::EditorInput::OnFileDropped(const char* path) {
    std::filesystem::path fs_path = path;
    std::filesystem::path ext = fs_path.extension();
    E64::Log::debug(std::string(path));

    if (ext == ".obj") {
        AssetImporter importer;
        E64::ECS::Mesh mesh = importer.importMesh(fs_path);
        E64::ECS::MeshComponent comp = E64::Engine::ctx->asset_manager->addMesh(mesh);
        E64::ECS::MeshComponent* mesh_comp = E64::ECS::ComponentManager::getComponent<E64::ECS::MeshComponent>(selected_entity);
        if (mesh_comp) mesh_comp->mesh_handle = comp.mesh_handle;
        else E64::Log::error("Entity Has No Mesh Component!\nCreate a Mesh Component for this Entity to Assign a Mesh to it.");
    }
    else if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
        E64::ECS::MeshComponent* mesh_comp = E64::ECS::ComponentManager::getComponent<E64::ECS::MeshComponent>(selected_entity);
        E64::ECS::Mesh* mesh = E64::Engine::ctx->asset_manager->getMesh(mesh_comp->mesh_handle);
        // mesh->texture_path = TBO(fs_path);
        // mesh->texture_path.upload();
    }
}