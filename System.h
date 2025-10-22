#pragma once

#include <set>
#include "Entity.h"

class ECSCoordinator;

class System
{
public:

    std::set<Entity> entities; // システムが管理するエンティティ群

    virtual ~System() = default;

    virtual void Update(ECSCoordinator& ecs) = 0;
};