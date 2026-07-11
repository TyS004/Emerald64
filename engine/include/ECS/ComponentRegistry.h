#ifndef COMPONENT_REGISTRY_H
#define COMPONENT_REGISTRY_H

#include "ECS/Component.h"
#include "ECS/Entity.h"

#include "E64Core.h"

namespace E64{
    namespace ECS{
        struct E64_API ComponentRegistryBase{
            inline static std::unordered_map
                            <std::string,
                                std::pair<std::function<void(json&, ECS::Entity)>,
                                          std::function<void(const json&, ECS::Entity)>>> handlers;
            inline static std::unordered_map<std::string, std::function<void()>> flush_fns;
            inline static int registered_components = 0;
        };
        
        template <typename T>
        struct E64_API ComponentRegistry : ComponentRegistryBase{
            inline static std::vector<T> registry {};
            inline static ComponentMask mask;

            inline static std::unordered_map<ECS::Entity, uint32_t> entity_to_idx;
            inline static std::unordered_map<uint32_t, ECS::Entity> idx_to_entity;

            static void registerComponent(const std::string& name){
                handlers[name] = { &serialize, &deserialize };
                flush_fns[name] = {&flush};
                mask = 1 << registered_components++;
                E64::Log::info("Registered " + name + " Component into Registry");
            }

            static void serialize(json& json, ECS::Entity e){
                if(ComponentManager::hasComponent<T>(e))
                {
                    std::cout << "SERIALIZING: " << mask << std::endl;
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

                ComponentManager::addComponent<T>(e, comp);
            }

            static void flush(){
                entity_to_idx.clear();
                idx_to_entity.clear();
                registry.clear();
            }
        };
    }
}

#endif