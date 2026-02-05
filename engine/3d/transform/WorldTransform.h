#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

#include "MyMath.h"

// \brief WorldTransform
// ワールド座標変換を管理するクラス。
// スケール、回転、平行移動の情報を保持し、行列計算によるワールド行列(matWorld_)の更新を行う。
// クォータニオンによる回転もサポートし、親子関係(parent)による階層変換も可能。
// DirectX12の定数バッファ(transformResource_)を介してGPUに変換行列を転送できる。

namespace Kouro
{
	struct ConstBufferDataWorldTransform
	{
		Matrix4x4 matWorld;
		Matrix4x4 matWorldInverse;
	};



	class WorldTransform
	{
	public:
		/// \brief 初期化
		void Initialize();

		/// \brief 更新
		void UpdateMatrix();

		/// \brief 行列転送
		void TransferMatrix();

		/**
		* \brief  GetTransformResource ワールド変換のリソース取得
		* \return transformResource_ ワールド変換のリソース
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetTransformResource() { return transformResource_; }

		/**
		* \brief  ワールド座標での位置を取得
		*/
		const Vector3 GetWorldPosition() const { return Vector3(matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2]); }

		const Vector3 GetTranslate() const { return transform.translate; }
		const Vector3 GetRotate() const { return transform.rotate; }
		const Vector3 GetScale() const { return transform.scale; }

		/**
		* \brief  GetWorldRotationMatrix ワールド回転行列の取得
		* \return ワールド回転行列
		*/
		Matrix4x4 GetWorldRotationMatrix() const;

		/**
		* \brief  GetWorldMatrix ワールド行列の取得
		* \return ワールド行列
		*/
		Matrix4x4 GetWorldMatrix() const { return matWorld_; }

		void SetTranslate(const Vector3& translate) { transform.translate = translate; }
		void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
		void SetScale(const Vector3& scale) { transform.scale = scale; }

		/**
		* \brief  SetParent 親ワールド変換の設定
		* \param  parent 親ワールド変換のポインタ
		*/
		void SetParent(const WorldTransform* parent) { parent_ = parent, this->UpdateMatrix(); }

	private:

		Transform transform;

		QuaternionTransform quaternionTransform;

		Matrix4x4 matWorld_ = MakeIdentity4x4();

		Matrix4x4 matWorldInverse_ = MakeIdentity4x4();

		bool useQuaternion_ = false;

		Vector3 deltaRotate_ = { 0.0f,0.0f,0.0f };

	private:

		// 定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_;

		ConstBufferDataWorldTransform* transformData_ = nullptr;

		const WorldTransform* parent_ = nullptr;

		Quaternion preRotate_ = {};

	};
}
