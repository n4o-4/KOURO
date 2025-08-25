﻿#pragma once
#include "BaseLineObject.h"

#include "OBBCollider.h"

class OBBLineObject : public BaseLineObject
{
public: // 公開メンバ関数

	~OBBLineObject() override = default;

	/**
	 * \brief  Initialize 初期化
	 */

	void Initialize(DirectXCommon* dxCommon)override;

	/**
	 * \brief  Update 更新
	 */

	void Update()override;

	/**
	 * \brief  Draw 描画
	 * \param  viewProjection ビュープロジェクション行列
	 */

	void Draw(const ViewProjection& viewProjection)override;

	/**
	 * \brief  SetAABBCollider AABBColliderを設定する
	 * \param  aabbCollider AABBColliderへのポインタ
	 */

	void SetOBBCollider(OBBCollider* obbCollider) { obbCollider_ = obbCollider; }

private: // 非公開メンバ関数

	/**
	 * \brief  CreateVertexResource 頂点リソースを生成する
	 */

	void CreateVertexResource();

	/**
	 * \brief  CreateVertexBufferView 頂点バッファビューを生成する
	 */

	void CreateVertexBufferView();

private: // 非公開メンバ変数

	OBBCollider* obbCollider_ = nullptr; //!< AABBColliderへのポインタ

};

