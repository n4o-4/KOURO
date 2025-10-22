#pragma once

#include "EntityManager.h"
#include "ComponentArray.h"

template<typename T>

class ComponentManager {
public:

    void RegisterComponent()
    {
        std::type_index typeIndex = typeid(T);
        assert(componentArrays_.find(typeIndex) == componentArrays_.end() &&
            "Component already registered!");
        componentArrays_[typeIndex] = std::make_shared<ComponentArray<T>>();
    }

    void AddComponent(Entity entity, const T& component)
    {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    void RemoveComponent(Entity entity)
    {
        GetComponentArray<T>()->RemoveData(entity);
    }

    T& GetComponent(Entity entity)
    {
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto& [type, componentArray] : componentArrays_) {
            componentArray->EntityDestroyed(entity);
        }
    }

private:

    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        std::type_index typeIndex = typeid(T);
        assert(componentArrays_.find(typeIndex) != componentArrays_.end() &&
            "Component not registered before use!");
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays_[typeIndex]);
    }

private:

    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays_;
    
};