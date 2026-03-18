#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

namespace Kouro
{
	struct PointLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // ライトの位置
		float intensity; // 輝度
		float radius; // ライトの届く最大距離
		float decay; // 減衰率
		float padding[2];
	};


	// \brief PointLight  
	// 点光源（ランプや炎のような位置に依存する光）を管理するクラス。  
	// 光の色・位置・強度・半径・減衰を設定し、シーン内で局所的な照明効果を提供する。

	class PointLight
	{
	public:

		/// \brief 初期化
		void Initilize();

		/// \brief 更新
		void Update();

		/**
		* \brief  ライトの色を取得する
		* \return ライトの色
		*/
		const Vector4& GetColor() const { return color_; }

		/**
		* \brief  ライトの位置を取得する
		* \return ライトの位置
		*/
		const Vector3& GetPosition() const { return position_; }

		/**
		* \brief  ライトの強度を取得する
		* \\return ライトの強度
		*/
		const float GetIntensity() const { return intensity_; }

		/**
		* \brief  ライトの届く最大距離を取得する
		* \return ライトの届く最大距離
		*/
		const float GetRadius() const { return radius_; }

		/**
		* \brief  ライトの減衰率を取得する
		* \return ライトの減衰率
		*/
		const float GetDecay() const { return decay_; }

		/**
		* \brief  ライトの色を設定する
		* \param  color ライトの色
		*/
		void SetColor(const Vector4& color) { color_ = color; }

		/**
		* \brief  ライトの位置を設定する
		* \param  position ライトの位置
		*/
		void SetPosition(const Vector3& position) { position_ = position; }

		/**
		* \brief  ライトの強度を設定する
		* \param  intensity ライトの強度
		*/
		void SetIntensity(float intensity) { intensity_ = intensity; }

		/**
		* \brief  ライトの届く最大距離を設定する
		* \param  radius ライトの届く最大距離
		*/
		void SetRadius(float radius) { radius_ = radius; }

		/**
		* \brief  ライトの減衰率を設定する
		* \param  decay ライトの減衰率
		*/
		void SetDecay(float decay) { decay_ = decay; }

		/**
		* \brief  ポイントライトのresourceを取得する
		* \return pointLightResource
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetPointLightResource() { return pointLightResource_; }

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_;

		PointLightData* pointLightData_ = nullptr;

		

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_;

		PointLightData* pointLightData_ = nullptr;

		Vector4 color_;
		Vector3 position_;
		float intensity_;
		float radius_;
		float decay_;
	};
}