#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <fbxsdk.h>
#include "ECS/Componet.h"

class FBXParser{
    public:
        FBXParser(const char* path);
        ~FBXParser();

        GCGameEngine::ECS::Mesh* getMesh();
    private:
        FbxMesh* getRawMesh(FbxNode* pNode);
        FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
        void PrintTabs();
        void PrintAttribute(FbxNodeAttribute* pAttribute);

        void parseIndicies();
        void parseVertices();

        FbxManager* sdk_manager;
        FbxImporter* importer;
        FbxScene* scene;

        FbxMesh* raw_mesh;
};

#endif 