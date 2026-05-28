#include "Serialization/MeshSerializer.h"

E64::MeshSerializer::MeshSerializer(){

}

E64::MeshSerializer::~MeshSerializer(){

}

void E64::MeshSerializer::serialize(E64::ECS::Mesh* mesh){
    std::filesystem::path path = mesh->path;
    std::ofstream file(path.replace_extension(".e64mesh"), std::ios::binary | std::ios::trunc);
    if(!file) { E64::Log::error("Could Not Create Mesh Binary Asset File"); return; }

    file << *mesh;

    file.close();
}

E64::ECS::Mesh E64::MeshSerializer::deserialize(){
    std::filesystem::path path = "../assets/meshes/sphere.e64mesh";
    std::ifstream file(path, std::ios::binary);
    if(!file) { E64::Log::error("Could Not Binary Open " + path.string()); return E64::ECS::Mesh(); }  

    E64::ECS::Mesh mesh;
    mesh.path = path.string();
    file >> mesh;

    file.close();
    return mesh;
}