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

	Kouro::Vector3 translate = worldTransform_->GetTranslate();

	worldTransform_->SetTranslate(translate + velocity_);

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

	Kouro::Matrix4x4 matWorld = worldTransform_->GetWorldMatrix();

	worldPosition_ = 
	{
		matWorld.m[3][0],
		matWorld.m[3][1],
		matWorld.m[3][2]
	};

	return worldPosition_;
}

void BaseCharacter::DrawImGui()
{
#ifdef _DEBUG
#endif 

}