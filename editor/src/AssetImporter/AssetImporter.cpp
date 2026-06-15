#include "AssetImporter/AssetImporter.h"

Editor::AssetImporter::AssetImporter(){

}

Editor::AssetImporter::~AssetImporter(){

}

E64::Mesh Editor::AssetImporter::importMesh(std::string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType           |
        aiProcess_FlipUVs
    );

    if (nullptr == scene) {
        E64::Log::error(importer.GetErrorString());
        return E64::Mesh{};
    }

    // Walk node tree to find first mesh
    std::vector<aiMesh*> raw_meshes = {};
    std::function<void(aiNode*)> findMeshes = [&](aiNode* node) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
            raw_meshes.push_back(scene->mMeshes[node->mMeshes[i]]);
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            findMeshes(node->mChildren[i]);
    };
    findMeshes(scene->mRootNode);

    if (raw_meshes.empty()) {
        E64::Log::error("ASSET IMPORTER ERROR: Mesh not Found in File");
        return E64::Mesh{};
    }

    E64::Mesh mesh{};
    std::vector<E64::Vertex> vertices;
    std::vector<uint32_t> indices;
    int index_offset = 0;

    for(aiMesh* raw_mesh : raw_meshes){

        E64::Log::debug(std::to_string(raw_mesh->mNumVertices)  + " Vertices from " + path);
        E64::Log::debug(std::to_string(raw_mesh->mNumFaces)     + " Polygon Count");
        E64::Log::debug(std::to_string(raw_mesh->mNumFaces * 3) + " Index Count");

        // --- Vertex Parsing ---
        bool has_uvs = raw_mesh->HasTextureCoords(0);
        if (!has_uvs) E64::Log::error("No UVs Found in Mesh");

        int num_poly_verts = raw_mesh->mNumFaces * 3;
        int start_vi = vertices.size();
        vertices.resize(start_vi + num_poly_verts);

        int vi = start_vi;
        for (unsigned int f = 0; f < raw_mesh->mNumFaces; f++) {
            E64::Log::info("Faces :" + std::to_string(f));
            const aiFace& face = raw_mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++, vi++) {
                unsigned int idx = face.mIndices[j];

                vertices[vi].pos.x = raw_mesh->mVertices[idx].x;
                vertices[vi].pos.y = raw_mesh->mVertices[idx].y;
                vertices[vi].pos.z = raw_mesh->mVertices[idx].z;

                vertices[vi].uv.x = has_uvs ? raw_mesh->mTextureCoords[0][idx].x : 0.f;
                vertices[vi].uv.y = has_uvs ? raw_mesh->mTextureCoords[0][idx].y : 0.f;

                vertices[vi].color = { 1.0f, 0.0f, 0.0f, 1.0f };
            }
        }

        for (int i = 0; i < num_poly_verts; i++){
            indices.push_back(index_offset + i);
        }
        index_offset += num_poly_verts;

        E64::Log::info(std::to_string(num_poly_verts) + " total indices");
        E64::Log::debug(path);
    }
    mesh.vertices = vertices;
    mesh.indices = indices;

    E64::MeshSerializer serializer;
    std::filesystem::path fs_path = path;
    serializer.serialize(mesh, fs_path.replace_extension(".e64mesh"));

    return mesh;
}