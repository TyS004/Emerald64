#include "ECS/ComponetManager.h"

using namespace E64;
using namespace ECS;
 
std::vector<TransformComponet> ComponetManager::transform_componets = {};
std::vector<MeshComponet> ComponetManager::mesh_componets = {};

template <> 
TransformComponet* ComponetManager::getComponet<TransformComponet>(Entity e){
    if(e < 0) return nullptr;
    if(!(EntityManager::entity_index[e] & TRANSFORM)) { 
        std::cout << "No Transform Componet" << std::endl; 
        return nullptr; 
    }
    return &transform_componets[e];
}

template <>
MeshComponet* ComponetManager::getComponet<MeshComponet>(Entity e){
    if(e < 0) return nullptr;
    if(!(EntityManager::entity_index[e] & MESH)) { 
        std::cout << "No Mesh Componet" << std::endl; 
        return nullptr; 
    }
    return &mesh_componets[e];   
}

template <>
bool ComponetManager::hasComponet<TransformComponet>(Entity e){
    if(e < 0) return false;
    return EntityManager::entity_index[e] & TRANSFORM;
}

template <>
bool ComponetManager::hasComponet<MeshComponet>(Entity e){
    if(e < 0) return false;
    return EntityManager::entity_index[e] & MESH;
}

template <>
void ComponetManager::addComponet<TransformComponet>(Entity e, ECS::TransformComponet comp){
    if(e < 0) return;
    EntityManager::entity_index[e] |= TRANSFORM;
    transform_componets.push_back(comp);
}

template <>
void ComponetManager::addComponet<MeshComponet>(Entity e, ECS::MeshComponet comp){
    if(e < 0) return;
    EntityManager::entity_index[e] |= MESH;
    mesh_componets.push_back(comp);
}