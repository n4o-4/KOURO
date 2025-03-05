#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	offset_ = { 0.0f, 2.0f, -30.0f };

	BaseCamera::Initialize();
}

void FollowCamera::Update()
{
	if (target_ == nullptr)
	{
		assert(0);
	}

	viewProjection_->transform.translate = target_->transform.translate + offset_;


	BaseCamera::Update();
}
