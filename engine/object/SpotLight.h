#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"
#include "MyMath.h"

#include "DirectXCommon.h"

// \brief SpotLight  
// スポットライト（特定方向に円錐状に光を放つ光源）を管理するクラス。  
// 色・位置・方向・強度・距離・減衰・角度情報を設定し、局所的な照明や演出に使用される。

namespace Kouro
{
	struct SpotLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // ライトの位置
		float intensity; // 輝度
		Vector3 direction; // ライトの方向
		float distance; // ライトの届く最大距離
		float decay; // 減衰率
		float cosAngle; // スポットライトの余弦
		float cosFalloffStart;
		float padding[2];
	};

	class SpotLight
	{
	public:

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
		* \brief  ライトの位置を取得する
		* \return ライトの位置
		*/
		const Vector3& GetPosition() const { return position_; }

		/**
		* \brief  ライトの強度を取得する
		* \return ライトの強度
		*/
		const float GetIntensity() const { return intensity_; }

		/**
		* \brief  ライトの方向を取得する
		* \return ライトの方向
		*/
		const Vector3& GetDirection() const { return direction_; }

		/**
		* \brief  ライトの届く最大距離を取得する
		* \return ライトの届く最大距離
		*/
		const float GetDistance() const { return distance_; }

		/**
		* \brief  ライトの減衰率を取得する
		* \return ライトの減衰率
		*/
		const float GetDecay() const { return decay_; }

		/**
		* \brief  スポットライトの余弦を取得する
		* \return スポットライトの余弦
		*/
		const float GetCosAngle() const { return cosAngle_; }

		/**
		* \brief  スポットライトの減衰開始点の余弦を取得する
		* \return スポットライトの減衰開始点の余弦
		*/
		const float GetCosFalloffStart() const { return cosFalloffStart_; }

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
		* \brief  ライトの方向を設定する
		* \param  direction ライトの方向
		*/
		void SetDirection(const Vector3& direction) { direction_ = direction; }

		/**
		* \brief  ライトの届く最大距離を設定する
		* \param  distance ライトの届く最大距離
		*/
		void SetDistance(float distance) { distance_ = distance; }

		/**
		* \brief  ライトの減衰率を設定する
		* \param  decay ライトの減衰率
		*/
		void SetDecay(float decay) { decay_ = decay; }

		/**
		* \brief  スポットライトの余弦を設定する
		* \param  cosAngle スポットライトの余弦
		*/
		void SetCosAngle(float cosAngle) { cosAngle_ = cosAngle; }

		/**
		* \brief  スポットライトの減衰開始点の余弦を設定する
		* \param  cosFalloffStart スポットライトの減衰開始点の余弦
		*/
		void SetCosFalloffStart(float cosFalloffStart) { cosFalloffStart_ = cosFalloffStart; }

		/**
		* \brief  スポットライトのリソースを取得する
		* \return spotLightResource
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> GetSpotLightResource() { return spotLightResource_; }

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_;

		SpotLightData* spotLightData_ = nullptr;

		Vector4 color_;
		Vector3 position_;
		float intensity_;
		Vector3 direction_;
		float distance_;
		float decay_;
		float cosAngle_;
		float cosFalloffStart_;
	};
}
