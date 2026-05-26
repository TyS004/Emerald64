#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace E64{
    namespace ECS{
        struct ComponentRegistryBase{
            inline static std::unordered_map
                            <std::string,
                                std::pair<std::function<void(json&, ECS::Entity)>,
                                          std::function<void(const json&, ECS::Entity)>>> handlers;
        };
        
        template <typename T>
        struct ComponentRegistry : ComponentRegistryBase{
            inline static std::vector<T> registry {};

            inline static std::unordered_map<ECS::Entity, uint32_t> entity_to_idx;
            inline static std::unordered_map<uint32_t, ECS::Entity> idx_to_entity;

            static void registerComponent(const std::string& name){
                handlers[name] = { &serialize, &deserialize };
            }

            static void serialize(json& json, ECS::Entity e){
                if(ECS::EntityManager::entity_index[e] & ECS::ComponentBit<T>::mask)
                {
                    std::cout << "SERIALIZING: " << ECS::ComponentBit<T>::mask << std::endl;
                    T* comp = &registry[entity_to_idx[e]];
                    if(comp == nullptr){
                        E64::Log::error("Component Not On Entity!");
                        return;
                    }
                    comp->serialize(json);
                }
            }
        
            static void deserialize(const json& comp_json, ECS::Entity e){
                T comp;
                comp.deserialize(comp_json);

                // REFACTOR JUST ADD COMPONENTMANAGEER
                // PROB NEED TO MAKE A ADD COMPOENT HERE IDK
                if(e < 0) { E64::Log::error("Entity Invalid!"); return; }
                EntityManager::entity_index[e] |= ComponentBit<T>::mask;
                registry.push_back(comp);

                entity_to_idx[e] = registry.size() - 1;
                idx_to_entity[registry.size() - 1] = e;
            }
        };
    }
}

#endif