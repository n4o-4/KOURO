#include "InputSystem.h"
#include "ECSCoordinator.h"

void InputSystem::Update(ECSCoordinator& ecs)
{
	// 入力に基づいてエンティティの処理を行う
	for (const auto& entity : entities)
	{
		//ecs.GetComponent<TransformComponent>(entity);
		// ここで各エンティティに対する入力処理を実装
		// 例: 移動、アクションなど
	}
}
