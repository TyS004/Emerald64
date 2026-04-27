#include "ECS/Entity.h"

using namespace GCGameEngine;
using namespace ECS;

std::unordered_map<Entity, ComponetMask> EntityManager::entity_index = {};

Entity GCGameEngine::ECS::EntityManager::createEntity(){
    ComponetMask mask = 0;
    Entity e = 0;

    entity_index[e] = mask;
    return e;
}

void GCGameEngine::ECS::EntityManager::deleteEntity(){

}