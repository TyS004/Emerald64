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
    if(sdk_manager) {
        sdk_manager->Destroy();
        sdk_manager = nullptr;
    }
}

void truncatePathName(char** path) {
    char* last = strrchr(*path, '/');

    if (last != NULL) {
        *path = last + 1;
    }
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
        E64::Log::debug(std::to_string(raw_mesh->GetControlPointsCount()) + " Control Points from " + std::string(path));
        E64::Log::debug(std::to_string(raw_mesh->GetPolygonCount()) + " Polygon Count");
        E64::Log::debug(std::to_string(raw_mesh->GetPolygonVertexCount()) + " Polygon Vertex / Index Count");
    }
 
    E64::ECS::Mesh* mesh = new E64::ECS::Mesh{};

    //Vertex Parsing
    int num_vertices = raw_mesh->GetPolygonVertexCount();
    E64::Vertex* vertices = new E64::Vertex[num_vertices];
    parseVertices(&vertices, num_vertices);
    mesh->vbo = new E64::VBO(vertices, num_vertices);

    //Index Parsing
    int num_indices = raw_mesh->GetPolygonVertexCount();
    int* poly_verts = raw_mesh->GetPolygonVertices();
    uint32_t indicies[num_indices];
    for(int i = 0; i < num_indices; ++i){
        std::cout << "Index: " << i << ": " << poly_verts[i] << std::endl;
        indicies[i] = i;
    }
    mesh->ibo = new E64::IBO(indicies, num_indices);
    E64::Log::info(std::to_string(num_indices));

    //Path Name Truncating 
    std::string trunc_path = path;
    char* trunc_path_buf = &trunc_path[0];
    truncatePathName(&trunc_path_buf);
    mesh->path = trunc_path_buf;

    mesh->texture = new E64::Texture();

    return mesh;
}

void FBXParser::parseVertices(E64::Vertex** vertices, int num_vertices){
    int* poly_verts = raw_mesh->GetPolygonVertices();
    FbxVector4* ctrl_pts = raw_mesh->GetControlPoints();
    
    FbxGeometryElementUV* uv_element = raw_mesh->GetElementUV(0);
    FbxLayerElementArrayTemplate<FbxVector2>* arr = nullptr;
    if(uv_element) arr = &uv_element->GetDirectArray();
    else E64::Log::error("No UVs Found in Mesh");

    for(int i = 0; i < num_vertices; ++i){
        (*vertices)[i].pos.x = ctrl_pts[poly_verts[i]].mData[0];
        (*vertices)[i].pos.y = ctrl_pts[poly_verts[i]].mData[1];
        (*vertices)[i].pos.z = ctrl_pts[poly_verts[i]].mData[2];

        if(arr != nullptr){
            (*vertices)[i].uv.x = arr->GetAt(i).mData[0];
            (*vertices)[i].uv.y = arr->GetAt(i).mData[1];
        }
        else{
            (*vertices)[i].uv.x = 0;
            (*vertices)[i].uv.y = 0;
        }

        std::cout << ctrl_pts[poly_verts[i]].mData[0] << ", " << ctrl_pts[poly_verts[i]].mData[1] << ", " << ctrl_pts[poly_verts[i]].mData[1] << std::endl;

        if((i % 2) == 0){
            (*vertices)[i].color.r = 1;
            (*vertices)[i].color.g = .5;
            (*vertices)[i].color.b = 0;
            (*vertices)[i].color.a = 1;
        }
        else{
            (*vertices)[i].color.r = .2;
            (*vertices)[i].color.g = 1;
            (*vertices)[i].color.b = .5;
            (*vertices)[i].color.a = 1;
        }
    }
}