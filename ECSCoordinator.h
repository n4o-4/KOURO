#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <bitset>
#include <ranges>

#include "EntityManager.h"
//#include "ComponentManager.h"
//#include "SystemManager.h"

class ECSCoordinator
{
public:

	Entity CreateEntity() { return Entity{ ++lastEntityID_ }; }

	template <typename T>
	
	void AddComponent(Entity e, T component); 

private:

	uint32_t lastEntityID_ = 0;

	std::unique_ptr<EntityManager> entityManager_;
	//std::unique_ptr<ComponentManager> componentManager_;
	//std::unique_ptr<SystemManager> systemManager_;

};

