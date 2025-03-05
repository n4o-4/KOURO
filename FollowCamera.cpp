#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	offset_ = { 0.0f,0.0f,-10.0f };

	BaseCamera::Initialize();
}

void FollowCamera::Update()
{
	/// �t�H���[�J����

	if (target_ == nullptr) {
		assert(0);
	}

	viewProjection_->transform.translate = target_->transform.translate + offset_;

	/// �ėp�ϐ��̍X�V
	BaseCamera::Update();
}
