#pragma once

#include <queue>
#include <bitset>
#include <array>

#include "Entity.h"

constexpr uint32_t MAX_ENTITIES = 5000;
constexpr uint32_t MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager
{
public:
    EntityManager();

    Entity CreateEntity();

    void DestroyEntity(Entity entity);

    void SetSignature(Entity entity, Signature signature) { signatures_[entity.GetID()] = signature; }

    Signature GetSignature(Entity entity) { return signatures_[entity.GetID()]; }
private:
    std::queue<uint32_t> availableEntities_;
    std::array<Signature, MAX_ENTITIES> signatures_;
    uint32_t livingEntityCount_ = 0;
};

