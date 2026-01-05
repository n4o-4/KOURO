#pragma once
// Engine
#include "Kouro.h"
#include "ObjectLine.h"
#include "BaseEntity.h"

// \brief BaseCharacter キャラクター基底クラス

class BaseCharacter : protected BaseEntity
{
private:

	struct HitActionData
	{
		const float kHitInterval_ = 0.001f; //!< 被弾アクション間隔

		float hitIntervalTimer_ = 0.0f; //!< 被弾アクションタイマー
	};

public: // 公開メンバ関数

	/**
	* \brief  Initialize 初期化
	* \param  LineModel 線モデル
	*/
	virtual void Initialize(LineModel* model);

	// \brief Update 更新
	virtual void Update();

	// \brief 描画
	virtual void Draw();

	/**
	* \brief  GetWorldTransform
	* \return WorldTransform* ワールドトランスフォームのポインタ
	*/
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }


	void SetColor(Vector4 color) { objectLine_->SetColor({color.x,color.y,color.z,color.w}); }


protected: // 派生用メンバ関数

	// \brief DrawImGui デバッグUIの描画
	void DrawImGui();

protected: // 派生用メンバ変数

	// ワイヤーモデル
	std::unique_ptr<ObjectLine> objectLine_ = nullptr;

	// 速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	uint32_t hp_ = 0;

	HitActionData actionData_ = {};
};

