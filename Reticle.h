#pragma once
#include "BaseEntity.h"

/// \brief  Reticle クロスヘア
class Reticle : public BaseEntity
{
public:

	/**
	* \brief  コンストラクタ
	* \param  sprite スプライト
	*/
	Reticle(Kouro::Sprite* sprite) : sprite_(sprite) {}

	/// \brief  初期化
	void Initialize();

	/// \brief  更新
	void Update(Kouro::Input input);

	/**
	* \brief  SetParent 親ワールド変換の設定
	* \param  parent 親ワールド変換のポインタ
	*/

	void SetParent(Kouro::WorldTransform* parent) { worldTransform_->SetParent(parent); }

private:

	Kouro::Vector2 screenPosition_ = { 0.0f, 0.0f }; // 画面上の位置

	Kouro::Sprite* sprite_;	// クロスヘアのスプライト

};

