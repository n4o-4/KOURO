#include "EntityManager.h"

EntityManager::EntityManager()
{
    for (uint32_t i = 1; i < MAX_ENTITIES; ++i) {
        availableEntities_.push(i);
    }
}

Entity EntityManager::CreateEntity()
{
    uint32_t id = availableEntities_.front();
    availableEntities_.pop();
    livingEntityCount_++;
    return Entity{ id };
}

void EntityManager::DestroyEntity(Entity entity)
{
    signatures_[entity.GetID()].reset();
    availableEntities_.push(entity.GetID());
    livingEntityCount_--;
}