#include "BaseCharacter.h"

void BaseCharacter::Initialize(Kouro::LineModel* model)
{
	///========================================
	/// オブジェクト3D
	
	// 生成と初期化
	objectLine_ = std::make_unique<Kouro::ObjectLine>();
	objectLine_->Initialize(model);

	///========================================
	/// ワールドトランスフォーム
	
	// 生成と初期化
	worldTransform_ = std::make_unique<Kouro::WorldTransform>();
	worldTransform_->Initialize();


	worldTransform_->useQuaternion_ = false;
	// 初期値の設定


	///========================================
	/// その他変数
	
	// 速度の初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグの初期化
	isAlive_ = true;
}

void BaseCharacter::Update()
{
	///========================================
	/// ワールドトランスフォーム

	// クォータニオン／行列どちらの変換を使うかで移動先を更新
	if (worldTransform_->useQuaternion_)
	{
		worldTransform_->quaternionTransform.translate += velocity_;
	}
	else
	{
		worldTransform_->transform.translate += velocity_;
	}

	// 更新
	worldTransform_->UpdateMatrix();
}

void BaseCharacter::Draw()
{
	///========================================
	/// オブジェクト3D

	// 描画
	objectLine_->Draw(worldTransform_.get());
}

const Kouro::Vector3& BaseCharacter::GetWorldPosition()
{
	worldTransform_->UpdateMatrix();
	Kouro::Vector3 worldPos = {
		worldTransform_->matWorld_.m[3][0],
		worldTransform_->matWorld_.m[3][1],
		worldTransform_->matWorld_.m[3][2]
	};
	return worldPos;
}

void BaseCharacter::DrawImGui()
{
#ifdef _DEBUG

	ImGui::Begin("Character Status");

	ImGui::DragFloat3("scale", &worldTransform_->transform.scale.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_->transform.rotate.x, 0.01f);
	ImGui::DragFloat3("translate", &worldTransform_->transform.translate.x, 0.01f);

#endif 

}