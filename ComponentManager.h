#pragma once
#include <unordered_map>
#include <memory>
#include <cassert>
#include <typeindex>

#include "EntityManager.h"

class IComponentArray 
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>

class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity entity, T component) {
        assert(entityToIndexMap_.find(entity.GetID()) == entityToIndexMap_.end());
        size_t newIndex = size_;
        entityToIndexMap_[entity.GetID()] = newIndex;
        indexToEntityMap_[newIndex] = entity.GetID();
        componentArray_[newIndex] = component;
        size_++;
    }

    void RemoveData(Entity entity) {
        assert(entityToIndexMap_.find(entity.GetID()) != entityToIndexMap_.end());
        size_t indexOfRemovedEntity = entityToIndexMap_[entity.GetID()];
        size_t indexOfLast = size_ - 1;
        componentArray_[indexOfRemovedEntity] = componentArray_[indexOfLast];

        uint32_t entityOfLast = indexToEntityMap_[indexOfLast];
        entityToIndexMap_[entityOfLast] = indexOfRemovedEntity;
        indexToEntityMap_[indexOfRemovedEntity] = entityOfLast;

        entityToIndexMap_.erase(entity.GetID());
        indexToEntityMap_.erase(indexOfLast);
        size_--;
    }

    T& GetData(Entity entity) {
        assert(entityToIndexMap_.find(entity.GetID()) != entityToIndexMap_.end());
        return componentArray_[entityToIndexMap_[entity.GetID()]];
    }

    void EntityDestroyed(Entity entity) override {
        if (entityToIndexMap_.find(entity.GetID()) != entityToIndexMap_.end()) {
            RemoveData(entity);
        }
    }

private:

    std::array<T, MAX_ENTITIES> componentArray_;
    std::unordered_map<uint32_t, size_t> entityToIndexMap_;
    std::unordered_map<size_t, uint32_t> indexToEntityMap_;
    size_t size_ = 0;

};

class ComponentManager
{

};

