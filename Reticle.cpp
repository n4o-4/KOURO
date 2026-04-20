#include "Reticle.h"

void Reticle::Initialize()
{
	worldTransform_->SetTranslate(kReticleOffset_);

	// ビューポート行列の作成（仮）
	viewportMatrix_ = Kouro::MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f);
}

void Reticle::Update(Kouro::Input* input)
{
	// ワールド変換の更新
	worldTransform_->UpdateMatrix();

	// ワールド座標を取得
	Kouro::Vector3 worldPosition = worldTransform_->GetWorldPosition();

	// ビュー行列とプロジェクション行列を取得
	Kouro::Matrix4x4 viewMatrix = cameraManager_->GetActiveCamera()->GetViewProjection().GetViewMatrix();

	// ビュー行列とプロジェクション行列を掛け合わせる
	Kouro::Matrix4x4 projectionMatrix = cameraManager_->GetActiveCamera()->GetViewProjection().GetProjectionMatrix();
	
	// ビュー行列、プロジェクション行列、ビューポート行列を掛け合わせる
	Kouro::Matrix4x4 viewProjectionViewportMatrix = viewMatrix * projectionMatrix * viewportMatrix_;

	// ワールド座標をスクリーン座標に変換
	Kouro::Vector3 screenPos = Kouro::MyMath::Transform(worldPosition, viewProjectionViewportMatrix);

	// スクリーン座標をクロスヘアの位置に設定
	screenPosition_.x = screenPos.x;
	screenPosition_.y = screenPos.y;

	sprite_->SetPosition(screenPosition_);
}
