#include "ECS/Entity.h"

using namespace E64;
using namespace ECS;

std::unordered_map<Entity, ComponetMask> EntityManager::entity_index = {};

static uint32_t id = 0;

Entity E64::ECS::EntityManager::createEntity(){
    ComponetMask mask = 0;
    Entity e = id++;

    entity_index[e] = mask;
    return e;
}

Entity E64::ECS::EntityManager::createEntity(ECS::Entity e){
    ComponetMask mask = 0;

    entity_index[e] = mask;
    return e;
}

void E64::ECS::EntityManager::deleteEntity(){

}

void E64::ECS::EntityManager::flushEntites(){
    entity_index.clear();
    id = 0;
}