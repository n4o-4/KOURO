#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	offset_ = { 0.0f,0.0f,-10.0f };

	BaseCamera::Initialize();
}

void FollowCamera::Update()
{
	/// フォローカメラ

	if (target_ == nullptr) {
		assert(0);
	}

	viewProjection_->transform.translate = target_->transform.translate + offset_;

	/// 汎用変数の更新
	BaseCamera::Update();
}
