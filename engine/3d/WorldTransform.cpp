#include "WorldTransform.h"

void WorldTransform::Initialize()
{

	transformResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));

	// データを書き込むためのアドレスを取得
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform.scale = { 1.0f,1.0f,1.0f };

	transform.rotate = { 0.0f,0.0f,0.0f };

	transform.translate = { 0.0f,0.0f,0.0f };

	quaternionTransform.scale = { 1.0f,1.0f,1.0f };

	quaternionTransform.rotate = { 0.0f,0.0f,0.0f ,1.0f };

	quaternionTransform.translate = { 0.0f,0.0f,0.0f };
}

void WorldTransform::UpdateMatrix()
{
	if (useQuaternion_)
	{
		Quaternion delta = EulerToQuaternion(deltaRotate_);

		quaternionTransform.rotate = qNormalize(Multiply(delta,quaternionTransform.rotate));

		matWorld_ = MakeAffineMatrixforQuater(quaternionTransform.scale, quaternionTransform.rotate, quaternionTransform.translate);
	}
	else
	{
		matWorld_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	}

	if (parent_)
	{
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;

	if (useQuaternion_)
	{
		preRotate_ = quaternionTransform.rotate;
	}
}

void WorldTransform::TransferMatrix()
{
	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;
}
