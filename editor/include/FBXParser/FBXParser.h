#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <fbxsdk.h>
#include <E64.h>

class FBXParser{
    public:
        FBXParser();
        ~FBXParser();

        E64::ECS::Mesh* getMesh(const char* path);
    private:
        FbxMesh* getRawMesh(FbxNode* pNode);
        FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
        void PrintTabs();
        void PrintAttribute(FbxNodeAttribute* pAttribute);

        void parseIndicies();
        void parseVertices(std::vector<E64::Vertex>* verticies);

        FbxManager* sdk_manager;
        FbxImporter* importer;
        FbxScene* scene;

        FbxMesh* raw_mesh;
};

#endif 