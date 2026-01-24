#pragma once
// Engine
#include "Kouro.h"

// 全てのオブジェクトの基底クラス

class BaseEntity
{
public: // 公開メンバ関数

	// \brief  Initialize 初期化
	void Initialize();

	// \brief  Update 更新
	void Update();

	/**
	 * \brief  GetWorldTransform ワールド変換情報を取得する
	 * \return worldTransform
	 */
	Kouro::WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

protected: // 派生用メンバ関数

	// \brief DrawImGui ImGui描画
	void DrawImGui();

public: // 派生用メンバ変数

	// ワールドトランスフォーム
	std::unique_ptr<Kouro::WorldTransform> worldTransform_;

	// 有効フラグ
	bool isAlive_ = true;
};

