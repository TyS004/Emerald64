#include "Serialization/MeshSerializer.h"

E64::MeshSerializer::MeshSerializer(){

}

E64::MeshSerializer::~MeshSerializer(){

}

void E64::MeshSerializer::serialize(E64::ECS::Mesh* mesh){
    std::filesystem::path path = mesh->obj_path;
    std::ofstream file(path.replace_extension(".e64mesh"), std::ios::binary | std::ios::trunc);
    if(!file) { E64::Log::error("Could Not Create Mesh Binary Asset File"); return; }

    file << *mesh;

    file.close();
}

E64::ECS::Mesh E64::MeshSerializer::deserialize(std::string path){
    std::filesystem::path fs_path = path;
    std::ifstream file(fs_path, std::ios::binary);
    if(!file) { E64::Log::error("Could Not Binary Open " + fs_path.string()); return E64::ECS::Mesh(); }  

    E64::ECS::Mesh mesh;
    mesh.obj_path = fs_path.string();
    file >> mesh;

    file.close();
    return mesh;
}