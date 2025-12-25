#include "WorldTransform.h"

void WorldTransform::Initialize()
{

	transformResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));

	// データを書き込むためのアドレスを取得
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	// 初期化
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.translate = { 0.0f,0.0f,0.0f };

	quaternionTransform.scale = { 1.0f,1.0f,1.0f };
	quaternionTransform.rotate = { 0.0f,0.0f,0.0f ,1.0f };
	quaternionTransform.translate = { 0.0f,0.0f,0.0f };
}

void WorldTransform::UpdateMatrix()
{
	// 回転の適用
	if (useQuaternion_)
	{
		// Quaternionでの回転を適用
		Quaternion delta = EulerToQuaternion(deltaRotate_);

		quaternionTransform.rotate = qNormalize(Multiply(delta,quaternionTransform.rotate));

		matWorld_ = MakeAffineMatrixforQuater(quaternionTransform.scale, quaternionTransform.rotate, quaternionTransform.translate);
	}
	else
	{
		// Euler角での回転を適用
		matWorld_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	}

	// 親がいる場合は親のワールド行列を掛ける
	if (parent_)
	{
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	// 逆行列の計算
	matWorldInverse_ = Inverse(matWorld_);

	// 定数バッファへ転送
	transformData_->matWorld = matWorld_;
	transformData_->matWorldInverse = matWorldInverse_;

	// 前回の回転を保存
	if (useQuaternion_)
	{
		preRotate_ = quaternionTransform.rotate;
	}
}

void WorldTransform::TransferMatrix()
{
	// 逆行列の計算
	matWorldInverse_ = Inverse(matWorld_);

	// 定数バッファへ転送
	transformData_->matWorld = matWorld_;
	transformData_->matWorldInverse = matWorldInverse_;
}
