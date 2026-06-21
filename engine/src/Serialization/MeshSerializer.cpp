#include "Serialization/MeshSerializer.h"
#include "Engine.h"

E64::MeshSerializer::MeshSerializer(){

}

E64::MeshSerializer::~MeshSerializer(){

}

float swap_float(float f){
    #ifdef E64_APPLE
        uint32_t val;
        memcpy(&val, &f, sizeof(float));
        val = __builtin_bswap32(val);
        memcpy(&f, &val, sizeof(float));
    #else 
        return f;
    #endif
}

void E64::MeshSerializer::serialize(E64::Mesh mesh, std::string path){
    std::filesystem::path fs_path = path;
    std::ofstream file(fs_path, std::ios::binary | std::ios::trunc);
    if(!file) { E64::Log::error("Could Not Create Mesh Binary Asset File"); return; }

    // file << mesh;
    E64::Log::info("Writing to E64Mesh...");
        
    uint32_t vcount   = mesh.vertices.size();
    uint32_t icount   = mesh.indices.size();

    file.write(reinterpret_cast<const char*>(&vcount),               sizeof(uint32_t));
    file.write(reinterpret_cast<const char*>(mesh.vertices.data()),  sizeof(Vertex) * vcount);
    file.write(reinterpret_cast<const char*>(&icount),               sizeof(uint32_t));
    file.write(reinterpret_cast<const char*>(mesh.indices.data()),   sizeof(uint32_t) * icount);

    file.close();
}

E64::Mesh E64::MeshSerializer::deserialize(std::string path){
    std::filesystem::path fs_path = path;
    std::ifstream file(fs_path, std::ios::binary);
    if(!file) { E64::Log::error("Could Not Binary Open " + fs_path.string()); return E64::Mesh(); }  

    E64::Mesh mesh;
    // file >> mesh;
    E64::Log::info("Reading from E64Mesh...");

    uint32_t vcount = 0;
    file.read(reinterpret_cast<char*>(&vcount), sizeof(uint32_t));
    #ifdef E64_APPLE
        if(E64::Engine::ctx->mode == N64_RUNTIME) vcount = __builtin_bswap32(vcount);
    #endif
    mesh.vertices.resize(vcount);
    file.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(Vertex) * vcount);
    if(E64::Engine::ctx->mode == N64_RUNTIME)
    {
        for(Vertex& v : mesh.vertices){
            v.pos.x = swap_float(v.pos.x);
            v.pos.y = swap_float(v.pos.y);
            v.pos.z = swap_float(v.pos.z);
    
            v.color.r = swap_float(v.color.r);
            v.color.g = swap_float(v.color.g);
            v.color.b = swap_float(v.color.b);
            v.color.a = swap_float(v.color.a);
            
            v.uv.x = swap_float(v.uv.x);
            v.uv.y = swap_float(v.uv.y);
        }
    }

    uint32_t icount = 0;
    file.read(reinterpret_cast<char*>(&icount), sizeof(uint32_t));
    #ifdef E64_APPLE
        if(E64::Engine::ctx->mode == N64_RUNTIME) icount = __builtin_bswap32(icount);
    #endif
        mesh.indices.resize(icount);

        file.read(reinterpret_cast<char*>(mesh.indices.data()), sizeof(uint32_t) * icount);
    #ifdef E64_APPLE
        if(E64::Engine::ctx->mode == N64_RUNTIME){
            for(uint32_t& i : mesh.indices){
                i = __builtin_bswap32(i);
            }
        }
    #endif
    E64::Log::info("Vertex Count: " + std::to_string(mesh.vertices.size()));
    E64::Log::info("Index Count: " + std::to_string(mesh.indices.size()));

    E64::Log::info(std::to_string(mesh.vertices[0].pos.x));
    E64::Log::info(std::to_string(mesh.vertices[0].pos.y));
    E64::Log::info(std::to_string(mesh.vertices[0].pos.z));

    E64::Log::info(std::to_string(mesh.indices[0]));
    E64::Log::info(std::to_string(mesh.indices[1]));
    E64::Log::info(std::to_string(mesh.indices[2]));

    file.close();
    return mesh;
}