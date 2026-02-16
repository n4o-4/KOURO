#include "BaseCamera.h"

namespace Kouro
{
	void BaseCamera::Initialize()
	{
		// worldTransformの生成と初期化
		worldTransform_ = std::make_unique<WorldTransform>();
		worldTransform_->Initialize();

		// viewProjectionの生成と初期化
		viewProjection_ = std::make_unique<ViewProjection>();
		viewProjection_->Initialize();

		// 初期化後に一度更新しておく
		viewProjection_->Update(*worldTransform_.get());
	}

	void BaseCamera::Update()
	{
		// 状態の更新
		worldTransform_->UpdateMatrix();

		// オフセット設定
		viewProjection_->SetOffset(shakeOffset_);

		// viewProjectionの更新
		viewProjection_->Update(*worldTransform_.get());
	}

	Vector3 BaseCamera::CalculationOffset()
	{
		// オフセットの初期化
		Kouro::Vector3 offset = offset_;

		// 回転行列の作成
		Kouro::Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->GetRotate());

		// オフセットに回転を適用
		offset = TransformNormal(offset, rotateMatrix);

		// 計算したオフセットを返す
		return offset;
	}

	void BaseCamera::ChangeState(std::unique_ptr<ICameraState> newState)
	{
		if (!newState) return;

		// もし既存のステートがあれば
		if (state_)
		{
			state_->Exit();
		}

		state_ = std::move(newState);


		state_->Enter();
	}
}