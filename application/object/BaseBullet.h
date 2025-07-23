#pragma once

#include "Kouro.h"

class BaseBullet
{
public: // 公開メンバ関数
	
	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */

	virtual void Initialize(Model* model, Vector3 spawnPos);

	// \brief  Update 更新

	virtual void Update();
	
	
	/**
	 * \brief  Draw 描画
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */

	virtual void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);



protected: // 派生用メンバ変数

	// 3Dモデル
	std::unique_ptr<Object3d> object3d_;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_;

	// 速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグ
	bool isActive_ = true;

	BaseCamera* camera_ = nullptr;
};

