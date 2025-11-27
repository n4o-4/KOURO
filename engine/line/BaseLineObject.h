#pragma once

#include "DirectXCommon.h"
#include "SrvManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

namespace LineDrawer
{
	struct VertexData // ライン描画用のVertexData
	{
		Vector4 position; // 頂点位置
		Vector4 color; // 頂点カラー
	};
}

// \brief BaseLineObject  
// ライン描画オブジェクトの基底クラス。  
// 頂点リソースやバッファビューを管理し、派生クラスで線形オブジェクトの描画処理を共通化する。

class BaseLineObject // ラインオブジェクトの基底クラス
{
public: // 公開メンバ関数

	virtual ~BaseLineObject() = default;

	/**
	 * \brief  Initialize 初期化
	 * \param  dxCommon DirectXCommonのインスタンス
	 */

	virtual void Initialize(DirectXCommon* dxCommon);

	/**
	 * \brief  Update 更新
	 */

	virtual void Update();

	/**
	 * \brief  Draw 描画
	 * \param  viewProjection ビュープロジェクション行列
	 */

	virtual void Draw(const ViewProjection& viewProjection);

protected:

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	LineDrawer::VertexData* vertexData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};

