#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

// \brief DirectionalLight  
// 平行光源（太陽光のような方向性のある光）を管理するクラス。  
// 光の色・方向・強度を設定し、シーン内での照明計算に使用される。

namespace Kouro
{
	class DirectionalLight
	{
	private:

		struct DirectionalLightData {
			Vector4 color;
			Vector3 direction;
			float intensity;
		};

	public: // 公開メンバ関数

		/// \brief 初期化
		void Initialize();

		/// \brief 更新
		void Update();

		/**
		* \brief  ライトの色を取得する
		* \return ライトの色
		*/
		const Vector4& GetColor() const { return color_; }

		/**
		* \brief  ライトの方向を取得する
		* \return ライトの方向
		*/
		const Vector3& GetDirection() const { return direction_; }

		/**
		* \brief  ライトの強度を取得する
		* \return ライトの強度
		*/
		const float GetIntensity() const { return intensity_; }

		/**
		* \brief  ライトの色を設定する
		* \param  color ライトの色
		*/
		void SetColor(const Vector4& color) { color_ = color; }

		/**
		* \brief  ライトの方向を設定する
		* \param  direction ライトの方向
		*/
		void SetDirection(const Vector3& direction) { direction_ = direction; }

		/**
		* \brief  ライトの強度を設定する
		* \param  intensity ライトの強度
		*/
		void SetIntensity(float intensity) { intensity_ = intensity; }

		/**
		* \brief  DirectionalLight用リソースの取得
		* \return directionalLightResource
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetDirectionalLightResource() { return directionalLightResource_; }

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource_;

		DirectionalLightData* directionalLightData_ = nullptr;

		Vector4 color_;
		Vector3 direction_;
		float intensity_;
	};
}