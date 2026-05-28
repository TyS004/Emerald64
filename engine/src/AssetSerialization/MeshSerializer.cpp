#include "AssetSerialization/MeshSerializer.h"

E64::MeshSerializer::MeshSerializer(){

}

E64::MeshSerializer::~MeshSerializer(){

}

void E64::MeshSerializer::serialize(E64::ECS::Mesh* mesh){
    std::ofstream file;
    file.open(mesh->path.replace_extension(".e64mesh"), std::ios::binary);
    if(!file) E64::Log::error("Could Not Create Mesh Binary Asset File");

    std::stringstream str;
    str << "Mesh Size: " << sizeof(E64::ECS::Mesh) << std::endl;
    str << "VBO Size: " << sizeof(E64::VBO*) << std::endl;
    str << "IBO Size: " << sizeof(E64::IBO*) << std::endl;
    str << "Texture Size: " << sizeof(E64::TBO*) << std::endl;
    str << "Path Size: " << sizeof(std::filesystem::path) << std::endl;
    E64::Log::info(str.str());

    file << *mesh;

    file.close();
}