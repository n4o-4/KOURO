#pragma once
#include "System.h"
class MoveSystem : public System
{
public:

	void Update() override
	{
		for (auto const& entity : entities)
		{
			// ‚±‚±‚Åentity‚ÌˆÊ’u‚ðXV‚·‚éˆ—‚ðŽÀ‘•
			 —á: auto& position = ecsCoordinator->GetComponent<Position>(entity);
			     auto& velocity = ecsCoordinator->GetComponent<Velocity>(entity);
			     position.x += velocity.x;
			     position.y += velocity.y;
		}
	}

};

