#include "FBXParser/FBXParser.h"

int numTabs = 0;

void FBXParser::PrintTabs() {
    for(int i = 0; i < numTabs; i++)
        printf("\t");
}

FbxString FBXParser::GetAttributeTypeName(FbxNodeAttribute::EType type) {
    switch(type) {
        case FbxNodeAttribute::eUnknown: return "unidentified";
        case FbxNodeAttribute::eNull: return "null";
        case FbxNodeAttribute::eMarker: return "marker";
        case FbxNodeAttribute::eSkeleton: return "skeleton";
        case FbxNodeAttribute::eMesh: return "mesh";
        case FbxNodeAttribute::eNurbs: return "nurbs";
        case FbxNodeAttribute::ePatch: return "patch";
        case FbxNodeAttribute::eCamera: return "camera";
        case FbxNodeAttribute::eCameraStereo: return "stereo";
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
        case FbxNodeAttribute::eLight: return "light";
        case FbxNodeAttribute::eOpticalReference: return "optical reference";
        case FbxNodeAttribute::eOpticalMarker: return "marker";
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case FbxNodeAttribute::eBoundary: return "boundary";
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case FbxNodeAttribute::eShape: return "shape";
        case FbxNodeAttribute::eLODGroup: return "lodgroup";
        case FbxNodeAttribute::eSubDiv: return "subdiv";
        default: return "unknown";
    }
}

void FBXParser::PrintAttribute(FbxNodeAttribute* pAttribute) {
    if(!pAttribute) return;

    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

FbxMesh* FBXParser::getRawMesh(FbxNode* pNode) {
    FbxMesh* mesh = nullptr;

    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get();
    FbxDouble3 rotation = pNode->LclRotation.Get();
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
        nodeName,
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
        );
    numTabs++;

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++){
        FbxNodeAttribute* attribute = pNode->GetNodeAttributeByIndex(i);
        if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh){
            FbxGeometryConverter converter(sdk_manager);
            mesh = (FbxMesh*)converter.Triangulate(pNode->GetMesh(), true);
            if(!mesh) { E64::Log::error("Mesh Not Found"); }
        }
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));
    }

    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        mesh = getRawMesh(pNode->GetChild(j));

    numTabs--;
    PrintTabs();
    printf("</node>\n");

    return mesh;
}

FBXParser::FBXParser(){
    sdk_manager = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
    sdk_manager->SetIOSettings(ios);
}

FBXParser::~FBXParser(){
    sdk_manager->Destroy();
}

E64::ECS::Mesh* FBXParser::getMesh(const char* path){
    importer = FbxImporter::Create(sdk_manager, "");
    if(!importer->Initialize(path, -1, sdk_manager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
        exit(-1);
    }

    scene = FbxScene::Create(sdk_manager, "myScene");
    importer->Import(scene);
    importer->Destroy();

    FbxNode* rootNode = scene->GetRootNode();
    raw_mesh = getRawMesh(rootNode);
    if(!raw_mesh) { return nullptr; }
    else {
        E64::Log::debug(std::to_string(raw_mesh->GetControlPointsCount()) + " Verticies from " + std::string(path));
        E64::Log::debug(std::to_string(raw_mesh->GetPolygonCount()) + " Polygon Faces");
    }

    E64::ECS::Mesh* mesh = new E64::ECS::Mesh{};

    int num_indices = raw_mesh->GetPolygonCount() * 3;
    int num_vertices = raw_mesh->GetControlPointsCount();

    E64::Vertex* vertices = new E64::Vertex[num_vertices];

    int* poly_verts = raw_mesh->GetPolygonVertices();
    for(int i = 0; i < raw_mesh->GetPolygonCount() * 3; i += 3){
        std::cout << "Polygon " << i / 3 << ": \t";
        std::cout << poly_verts[i] << " " << poly_verts[i + 1]<< " " << poly_verts[i + 2] << std::endl;
    }
    std::cout << std::endl;
    mesh->ibo = new E64::IBO((uint32_t*)poly_verts, num_indices);

    FbxVector4* raw_vertices = raw_mesh->GetControlPoints();
    for(int i = 0; i < num_vertices; ++i){
        std::cout << "Vertex " << i << ": \t";
        std::cout << raw_vertices[i].mData[0] << " " << raw_vertices[i].mData[1] << " " << raw_vertices[i].mData[2] << std::endl;

        vertices[i].pos.x = raw_vertices[i].mData[0];
        vertices[i].pos.y = raw_vertices[i].mData[1];
        vertices[i].pos.z = raw_vertices[i].mData[2];

        if((i % 2) == 0){
            vertices[i].color.r = 1;
            vertices[i].color.g = .5;
            vertices[i].color.b = 0;
            vertices[i].color.a = 1;
        }
        else{
            vertices[i].color.r = .2;
            vertices[i].color.g = 1;
            vertices[i].color.b = .5;
            vertices[i].color.a = 1;
        }
    }
    mesh->vbo = new E64::VBO(vertices, num_vertices);

    char buf[200];
    int i = 0;
    while(*path != '\0'){
        std::cout << i << std::endl;
        buf[i] = *path;
        if(*path == '/') i = 0;
        i++;
        path += 1;
    }
    buf[i] = '\0';
    mesh->path = buf;
    std::cout << buf << std::endl;

    return mesh;
}