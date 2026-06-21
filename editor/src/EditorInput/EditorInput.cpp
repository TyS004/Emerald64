// EditorInput.cpp
#include "EditorInput/EditorInput.h"
#include "AssetImporter/AssetImporter.h"

#include <E64.h>
#include <backends/imgui_impl_sdl3.h>
#include <filesystem>

Editor::EditorInput::EditorInput() : E64::SDLInput() {

}

void Editor::EditorInput::processEvent(SDL_Event& e) {
    if(!E64::Engine::ctx->window->isMouseLock()) ImGui_ImplSDL3_ProcessEvent(&e);

    E64::SDLInput::processEvent(e);
    if (e.type == SDL_EVENT_DROP_FILE)
        OnFileDropped(e.drop.data);
}

void Editor::EditorInput::OnKeyPressed(E64::Scancode key) {
    E64::Window* window = E64::Engine::ctx->window;
    if (key == E64::Scancode::Escape) E64::Engine::exit();

    switch (key) {
        case E64::Scancode::Z:
            if (window->isMouseLock()) window->setMouseLock(false);
            else { SDL_HideCursor();   window->setMouseLock(true); }
            break;
        case E64::Scancode::V:
            window->toggleVSync();
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

    if (ext == ".obj" || ext == ".glb" || ext == ".gltf" || ext == ".fbx") {
        E64::ECS::MeshComponent* mesh_comp = E64::ECS::ComponentManager::getComponent<E64::ECS::MeshComponent>(selected_entity);
        if (!mesh_comp){
            E64::Log::error("Entity Has No Mesh Component!\nCreate a Mesh Component for this Entity to Assign a Mesh to it.");
            return;
        }

        AssetImporter importer;
        E64::Mesh mesh = importer.importMesh(fs_path.string());

        std::filesystem::path rel = std::filesystem::relative(path, E64::Engine::ctx->root_dir);
        E64::AssetHandle handle = E64::Engine::ctx->asset_manager->loadMeshAsset(rel.replace_extension(".e64mesh").string());
        mesh_comp->mesh_handle = handle;
        mesh_comp->mesh_path = rel.replace_extension(".e64mesh").string();
    }

    else if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
        E64::ECS::MeshComponent* mesh_comp = E64::ECS::ComponentManager::getComponent<E64::ECS::MeshComponent>(selected_entity);
        if(!mesh_comp){
            E64::Log::error("Entity Has No Mesh Component!\nCreate a Mesh Component for this Entity to Assign a Texture to it.");
            return;
        }
        
        std::filesystem::path rel = std::filesystem::relative(path, E64::Engine::ctx->root_dir);
        E64::AssetHandle handle = E64::Engine::ctx->asset_manager->loadTextureAsset(rel.string());
        mesh_comp->tex_handle = handle;
        mesh_comp->texture_path = rel.string();
    }
}