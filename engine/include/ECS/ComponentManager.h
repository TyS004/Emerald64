#ifndef COMPONET_MANAGER_H
#define COMPONET_MANAGER_H

#include "ECS/Component.h"
#include "ECS/ComponentRegistry.h"
#include "ECS/Entity.h"

#include <vector>

namespace E64{
    namespace ECS{
        class ComponentManager{
            public:
                template <typename T>   
                static T* getComponent(Entity e){
                    if(!(EntityManager::entity_index[e] & ComponentBit<T>::mask)) { 
                        E64::Log::error("Component Not Found : ComponentManager::getComponent");
                        return nullptr; 
                    }
                    uint32_t i = ComponentRegistry<T>::entity_to_idx.at(e);
                    return &ComponentRegistry<T>::registry.at(i);
                }
                
                template <typename T>
                static std::vector<T>* getComponentRegistry(){
                    return &ComponentRegistry<T>::registry;
                }
                
                template <typename T>
                static bool hasComponent(Entity e){
                    return EntityManager::entity_index[e] & ComponentBit<T>::mask;
                }
                
                template <typename T>
                static void addComponent(Entity e, T comp){
                    if(hasComponent<T>(e)) { 
                        E64::Log::error("Entity Already has this Component!");
                        return; 
                    }

                    EntityManager::entity_index[e] |= ComponentBit<T>::mask;
                    ComponentRegistry<T>::registry.push_back(comp);

                    ComponentRegistry<T>::entity_to_idx[e] = ComponentRegistry<T>::registry.size() - 1;
                    ComponentRegistry<T>::idx_to_entity[ComponentRegistry<T>::registry.size() - 1] = e;
                }

                template <typename T>
                static void addComponent(Entity e){
                    T comp {};
                    if(hasComponent<T>(e)) { 
                        E64::Log::error("Entity Already has this Component!");
                        return; 
                    }

                    EntityManager::entity_index[e] |= ComponentBit<T>::mask;
                    ComponentRegistry<T>::registry.push_back(comp);

                    ComponentRegistry<T>::entity_to_idx[e] = ComponentRegistry<T>::registry.size() - 1;
                    ComponentRegistry<T>::idx_to_entity[ComponentRegistry<T>::registry.size() - 1] = e;
                }

                template <typename T>
                static void removeComponent(Entity e){
                    E64::Log::debug("Removing Component From Entity: " + std::to_string(e));

                    std::vector<T>* registry = &ComponentRegistry<T>::registry;
                    uint32_t del_i = ComponentRegistry<T>::entity_to_idx[e];
                    E64::Log::debug("DEL IDX: " + std::to_string(del_i));
                    ECS::Entity last_entity = ComponentRegistry<T>::idx_to_entity[registry->size() - 1];

                    std::swap((*registry)[del_i], registry->back());
                    registry->pop_back();

                    ComponentRegistry<T>::entity_to_idx[last_entity] = del_i;
                    ComponentRegistry<T>::entity_to_idx.erase(e);
                    ComponentRegistry<T>::idx_to_entity[del_i] = last_entity;
                    ComponentRegistry<T>::idx_to_entity.erase(registry->size());

                    EntityManager::entity_index[e] &= ~ComponentBit<T>::mask;
                }
                
                static void flushComponents(){
                    for(auto& [name, clear_fn] : ComponentRegistryBase::clear_fns){
                        clear_fn();
                    }
                }

                static void serialize(json& entity_json, ECS::Entity e){
                    for(auto& [name, fns] : ComponentRegistryBase::handlers){
                        json j;
                        fns.first(j, e);
                        if(!j.empty()) entity_json[name] = j;
                    }
                }

                static void deserialize(const json& entity_json, ECS::Entity e){
                    for(auto& [name, fns] : ComponentRegistryBase::handlers){
                        if(entity_json.contains(name)) {
                            /*E64::Log::info("JSON DESERIALZIE CONTAINS: " + name);
                            E64::Log::info("");*/
                            fns.second(entity_json[name], e);
                        }
                    }
                }
        };
    }
}

#endif