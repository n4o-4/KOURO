#pragma once
#include "Vectors.h"
#include <numbers>

/**
* \brief ライトの初期値
*/

namespace Kouro
{
	namespace LightDefault
	{
		constexpr Vector4 kDefaultColor = { 1.0f, 1.0f, 1.0f, 1.0f }; //!< 白色
		constexpr float   kDefaultIntensity = 1.0f;                   //!< 標準強度

		namespace DirectionalLight
		{
			constexpr Vector3 kDefaultDirection = { -1.0f, 0.0f, 0.0f }; //!< 下方向
		}

		namespace PointLight
		{
			constexpr Vector3 kDefaultPosition = { 0.0f, 0.0f, 0.0f }; //!< 原点
			constexpr float   kDefaultRadius = 2.8f;                   //!< 半径
			constexpr float   kDefaultDecay = 0.95f;                   //!< 減衰率
		}

		namespace SpotLight
		{
			constexpr Vector3 kDefaultPosition = { 0.0f, 0.0f, 0.0f };          //!< 原点
			constexpr Vector3 kDefaultDirection = { -1.0f, 0.0f, 0.0f };        //!< 下方向
			constexpr float   kDefaultDistance = 7.0f;                          //!< 有効距離
			constexpr float   kDefaultDecay = 2.0f;                             //!< 減衰率
			constexpr float   kDefaultAngle = std::numbers::pi_v<float> / 3.0f; //!< 開き角60度
			constexpr float   kDefaultCosFalloffStart = 1.0f;                   //!< コサイン減衰開始
		}
	}
}