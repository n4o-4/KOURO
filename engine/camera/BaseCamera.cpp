#include "BaseCamera.h"

void BaseCamera::Initialize()
{
	// videwProjectionの生成と初期化
	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection_->Initialize();

	// 初期化後に一度更新しておく

	viewProjection_->Update();

	worldMatrix = MakeAffineMatrix(viewProjection_->transform.scale, viewProjection_->transform.rotate, viewProjection_->transform.translate);
}

void BaseCamera::Update()
{
	worldMatrix = MakeAffineMatrix(viewProjection_->transform.scale, viewProjection_->transform.rotate, viewProjection_->transform.translate);

	viewProjection_->SetOffset(offset_);

	viewProjection_->Update();
}
