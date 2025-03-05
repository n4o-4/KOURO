#pragma once
#include "BaseCamera.h"

#include "WorldTransform.h"

class FollowCamera : public BaseCamera
{
public:

	// ������
	void Initialize();

	// �X�V����
	void Update();

	void SetTarget(WorldTransform* target) { target_ = target; }

private:

	WorldTransform* target_ = nullptr;

	Vector3 offset_ = {};
};

