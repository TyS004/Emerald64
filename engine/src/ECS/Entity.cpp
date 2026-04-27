#include "ECS/Entity.h"

using namespace GCGameEngine;
using namespace ECS;

std::unordered_map<Entity, ComponetMask> EntityManager::entity_index = {};

static uint32_t id = 0;

Entity GCGameEngine::ECS::EntityManager::createEntity(){
    ComponetMask mask = 0;
    Entity e = id++;

    entity_index[e] = mask;
    return e;
}

void GCGameEngine::ECS::EntityManager::deleteEntity(){

}