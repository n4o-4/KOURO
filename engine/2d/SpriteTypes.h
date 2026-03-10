#pragma once
#include "Vectors.h"
#include "Matrixs.h"

/// \brief Sprite関連の構造体を定義するヘッダーファイル
namespace Kouro::Sprite
{
	/// \brief  スプライトの頂点情報を格納する構造体
	struct VertexData
	{
		Vector4 position; //!< 頂点の位置
		Vector2 texcoord; //!< 頂点のテクスチャ座標
		Vector3 normal;   //!< 頂点の法線ベクトル
	};

	/// \brief  スプライトのマテリアル情報を格納する構造体
	struct MaterialData
	{
		Vector4 color;         //!< スプライトの色
		Matrix4x4 uvTransform; //!< UV変換行列（テクスチャ座標の変換に使用）
	};

	/// \brief  スプライトの変換行列を格納する構造体
	struct TransformData
	{
		Matrix4x4 WVP;   //!< ワールド・ビュー・プロジェクション行列
		Matrix4x4 World; //!< ワールド行列
	};
}