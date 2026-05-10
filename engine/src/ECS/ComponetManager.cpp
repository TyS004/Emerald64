#include "ECS/ComponetManager.h"
using namespace E64;
using namespace ECS;
 
std::vector<Transform*> ComponetManager::transforms = {};
std::vector<Mesh*> ComponetManager::meshes = {};

template <>
Transform* ComponetManager::getComponet<Transform>(Entity e){
    if(!(EntityManager::entity_index[e] & TRANSFORM)) { 
        std::cout << "No Transform Componet" << std::endl; 
        return nullptr; 
    }
    return transforms[e];
}

template <>
Mesh* ComponetManager::getComponet<Mesh>(Entity e){
    if(!(EntityManager::entity_index[e] & MESH)) { 
        std::cout << "No Mesh Componet" << std::endl; 
        return nullptr; 
    }
    return meshes[e];   
}

template <>
bool ComponetManager::hasComponet<Transform>(Entity e){
    return EntityManager::entity_index[e] & TRANSFORM;
}

template <>
bool ComponetManager::hasComponet<Mesh>(Entity e){
    return EntityManager::entity_index[e] & MESH;
}

template <>
void ComponetManager::addComponet<Transform*>(Entity e, Transform* transform){
    EntityManager::entity_index[e] |= TRANSFORM;
    transforms.push_back(transform);
}

template <>
void ComponetManager::addComponet<Mesh*>(Entity e, Mesh* mesh){
    EntityManager::entity_index[e] |= MESH;  
    meshes.push_back(mesh);
}

void ComponetManager::clean(){
    for(Transform* transform : transforms){
        delete transform;
    }

    for(Mesh* mesh : meshes){
        delete mesh;
    }
}