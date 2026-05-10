#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <fbxsdk.h>
#include "ECS/Componet.h"

class FBXParser{
    public:
        FBXParser();
        FBXParser(const char* path);
        ~FBXParser();

        E64::ECS::Mesh* getMesh();
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