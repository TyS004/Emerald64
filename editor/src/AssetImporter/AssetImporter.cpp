#include "AssetImporter/AssetImporter.h"

Editor::AssetImporter::AssetImporter(){}
Editor::AssetImporter::~AssetImporter(){}

E64::ECS::Mesh Editor::AssetImporter::importMesh(std::filesystem::path path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path.string(),
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType           |
        aiProcess_FlipUVs
    );

    if (nullptr == scene) {
        E64::Log::error(importer.GetErrorString());
        return E64::ECS::Mesh{};
    }

    // Walk node tree to find first mesh
    aiMesh* raw_mesh = nullptr;
    std::function<void(aiNode*)> findMesh = [&](aiNode* node) {
        if (raw_mesh) return;
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            raw_mesh = scene->mMeshes[node->mMeshes[i]];
            return;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            findMesh(node->mChildren[i]);
    };
    findMesh(scene->mRootNode);

    if (!raw_mesh) {
        E64::Log::error("Mesh Not Found");
        return E64::ECS::Mesh{};
    }

    E64::Log::debug(std::to_string(raw_mesh->mNumVertices)  + " Vertices from " + path.string());
    E64::Log::debug(std::to_string(raw_mesh->mNumFaces)     + " Polygon Count");
    E64::Log::debug(std::to_string(raw_mesh->mNumFaces * 3) + " Index Count");

    E64::ECS::Mesh mesh{};

    // --- Vertex Parsing ---
    bool has_uvs = raw_mesh->HasTextureCoords(0);
    if (!has_uvs) E64::Log::error("No UVs Found in Mesh");

    int num_poly_verts = raw_mesh->mNumFaces * 3;
    std::vector<E64::Vertex> vertices(num_poly_verts);

    int vi = 0;
    for (unsigned int f = 0; f < raw_mesh->mNumFaces; f++) {
        const aiFace& face = raw_mesh->mFaces[f];
        for (unsigned int j = 0; j < face.mNumIndices; j++, vi++) {
            unsigned int idx = face.mIndices[j];

            vertices[vi].pos.x = raw_mesh->mVertices[idx].x;
            vertices[vi].pos.y = raw_mesh->mVertices[idx].y;
            vertices[vi].pos.z = raw_mesh->mVertices[idx].z;

            vertices[vi].uv.x = has_uvs ? raw_mesh->mTextureCoords[0][idx].x : 0.f;
            vertices[vi].uv.y = has_uvs ? raw_mesh->mTextureCoords[0][idx].y : 0.f;

            vertices[vi].color = { 0.5f, 0.5f, 0.5f, 1.0f };
        }
    }
    mesh.vertices = vertices;

    // --- Index Parsing ---
    std::vector<uint32_t> indices(num_poly_verts);
    for (int i = 0; i < num_poly_verts; i++)
        indices[i] = i;
    mesh.indices = indices;

    E64::Log::info(std::to_string(num_poly_verts) + " total indices");

    mesh.path    = path;
    mesh.texture = E64::TBO();

    return mesh;
}