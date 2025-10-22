#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <bitset>
#include <ranges>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

template <typename T>

class ECSCoordinator
{
public:

	Entity CreateEntity() { return Entity{ ++lastEntityID_ }; }

	void AddComponent(Entity e, T component)
	{
		componentManager_->AddComponent<T>(e, component);
		auto signature = entityManager_->GetSignature(e);
		signature.set(componentManager_->GetComponentType<T>(), true);
		entityManager_->SetSignature(e, signature);
		systemManager_->EntitySignatureChanged(e, signature);
	}

private:

	uint32_t lastEntityID_ = 0;

	std::unique_ptr<EntityManager> entityManager_;
	std::unique_ptr<ComponentManager> componentManager_;
	std::unique_ptr<SystemManager> systemManager_;

};

