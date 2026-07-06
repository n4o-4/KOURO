#pragma once
// Standard Library
#include <memory>
// Engine
#include "WorldTransform.h"

// 全てのオブジェクトの基底クラス

class BaseEntity
{
public: // 公開メンバ関数

	/**
	* \brief  デストラクタ
	*/
	virtual ~BaseEntity() = default;

	// \brief  Initialize 初期化
	void Initialize();

	// \brief  Update 更新
	void Update();

	/**
	 * \brief  ワールド変換情報を取得する
	 * \return worldTransform
	 */
	Kouro::WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

	/**
	* \brief  有効かどうかを取得する
	* \return isValid_  true:有効 false:無効
	*/
	bool IsValid() const { return isValid_; }

	/**
	* \brief  更新、描画が有効かどうかを取得する
	* \return isActive_  true:有効 false:無効
	*/
	bool IsActive() const { return isActive_; }

protected: // 派生用メンバ関数

	// \brief DrawImGui ImGui描画
	void DrawImGui();

protected: // 派生用メンバ変数

	// ワールドトランスフォーム
	std::unique_ptr<Kouro::WorldTransform> worldTransform_;

	bool isValid_ = true; // 存在の有効フラグ
	bool isActive_ = true; // 更新、描画の有効フラグ
};

