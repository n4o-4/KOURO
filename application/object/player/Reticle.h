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
	Reticle(Kouro::Sprite* sprite, Kouro::WorldTransform* worldTransform, Kouro::CameraManager* cameraManager);

	/// \brief  初期化
	void Initialize();

	/**
	* \brief  更新
	* \param  input 入力のポインタ
	*/
	void Update(Kouro::Input* input = Kouro::Input::GetInstance());

	/**
	* \brief  GetScreenPosition 画面上の位置を取得
	* \return screenPosition_ 画面上の位置
	*/
	Kouro::Vector2 GetScreenPosition() const { return screenPosition_; }

private:

	Kouro::Sprite* sprite_ = nullptr;	// クロスヘアのスプライト

	Kouro::CameraManager* cameraManager_ = nullptr; // カメラマネージャーのポインタ

	Kouro::Vector2 screenPosition_ = { 0.0f, 0.0f }; // 画面上の位置

	const Kouro::Vector3 kReticleOffset_ = { 0.0f, 0.0f, 50.0f }; // クロスヘアのオフセット

	Kouro::Matrix4x4 viewportMatrix_ = Kouro::MakeIdentity4x4(); // ビューポート行列
};

