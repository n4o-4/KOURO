#pragma once

#include "Entity.h"

class IComponentArray
{
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IComponentArray() = default;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="entity"></param>

	virtual void EntityDestroyed(Entity entity) = 0;
};

