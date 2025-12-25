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
	// ワールド行列の計算
	worldMatrix = MakeAffineMatrix(viewProjection_->transform.scale, viewProjection_->transform.rotate, viewProjection_->transform.translate);

	// オフセット設定
	viewProjection_->SetOffset(offset_);

	// viewProjectionの更新
	viewProjection_->Update();
}
