#pragma once

#include <set>

#include "Entity.h"

class System {
public:
    std::set<Entity> entities; // このシステムが管理するエンティティ群
    virtual ~System() = default;

    virtual void Update() = 0;
};