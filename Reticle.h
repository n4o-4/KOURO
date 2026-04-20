#pragma once
#include "BaseEntity.h"
#include "CameraManager.h"

/// \brief  Reticle クロスヘア
class Reticle : public BaseEntity
{
public:

	/**
	* \brief  コンストラクタ
	* \param  sprite スプライト
	* \param  親ワールド変換のポインタ
	*/
	Reticle(Kouro::Sprite* sprite, Kouro::WorldTransform* worldTransform, Kouro::CameraManager* cameraManager) : sprite_(sprite), cameraManager_(cameraManager)
	{
		worldTransform_ = std::make_unique<Kouro::WorldTransform>();
		worldTransform_->Initialize();
		worldTransform_->SetParent(worldTransform);
	}

	/// \brief  初期化
	void Initialize();

	/// \brief  更新
	void Update(Kouro::Input* input = Kouro::Input::GetInstance());

	/**
	* \brief  SetParent 親ワールド変換の設定
	* \param  parent 親ワールド変換のポインタ
	*/

	void SetParent(Kouro::WorldTransform* parent) { worldTransform_->SetParent(parent); }

private:

	Kouro::Sprite* sprite_;	// クロスヘアのスプライト

	Kouro::CameraManager* cameraManager_; // カメラマネージャーのポインタ

	Kouro::Vector2 screenPosition_ = { 0.0f, 0.0f }; // 画面上の位置

	const Kouro::Vector3 kReticleOffset_ = { 0.0f, 0.0f, 50.0f }; // クロスヘアのオフセット

	Kouro::Matrix4x4 viewportMatrix_; // ビューポート行列
};

