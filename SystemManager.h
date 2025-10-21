#pragma once

#include <unordered_map>
#include <set>
#include <memory>
#include <typeindex>

#include "EntityManager.h"

class System {
public:
    std::set<Entity> entities;
    virtual ~System() = default;
};

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        const char* typeName = typeid(T).name();
        assert(systems_.find(typeName) == systems_.end());
        auto system = std::make_shared<T>();
        systems_.insert({ typeName, system });
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature) {
        const char* typeName = typeid(T).name();
        signatures_.insert({ typeName, signature });
    }

    void EntityDestroyed(Entity entity) {
        for (auto const& pair : systems_) {
            pair.second->entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto const& pair : systems_) {
            const auto& type = pair.first;
            const auto& system = pair.second;
            const auto& systemSignature = signatures_[type];

            if ((entitySignature & systemSignature) == systemSignature) {
                system->entities.insert(entity);
            }
            else {
                system->entities.erase(entity);
            }
        }
    }

private:
    std::unordered_map<const char*, Signature> signatures_;
    std::unordered_map<const char*, std::shared_ptr<System>> systems_;
};

