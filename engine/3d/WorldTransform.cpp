#include "WorldTransform.h"

void WorldTransform::Initialize()
{

	transformResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));

	// データを書き込むためのアドレスを取得
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform.scale = { 1.0f,1.0f,1.0f };

	transform.rotate = { 0.0f,0.0f,0.0f };

	transform.translate = { 0.0f,0.0f,0.0f };

}

void WorldTransform::UpdateMatrix()
{

	matWorld_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;

}

void WorldTransform::TransferMatrix()
{
	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;
}
