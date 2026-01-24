#include "SpotLight.h"
#include "InitialLightSetting.h"

namespace Kouro
{
	void SpotLight::Initialize()
	{
		// bufferResourceの生成
		spotLightResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(SpotLightData));

		// データをマップ
		spotLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

		// 初期値設定	
		color_ = LightDefault::kDefaultColor;
		position_ = LightDefault::SpotLight::kDefaultPosition;
		intensity_ = LightDefault::kDefaultIntensity;
		direction_ = Normalize(LightDefault::SpotLight::kDefaultDirection);
		distance_ = LightDefault::SpotLight::kDefaultDistance;
		decay_ = LightDefault::SpotLight::kDefaultDecay;
		cosAngle_ = std::cos(LightDefault::SpotLight::kDefaultAngle);
		cosFalloffStart_ = LightDefault::SpotLight::kDefaultCosFalloffStart;

		spotLightData_->color = color_;
		spotLightData_->position = position_;
		spotLightData_->intensity = intensity_;
		spotLightData_->direction = direction_;
		spotLightData_->distance = distance_;
		spotLightData_->decay = decay_;
		spotLightData_->cosAngle = cosAngle_;
		spotLightData_->cosFalloffStart = cosFalloffStart_;
	}

	void SpotLight::Update()
	{
		// データ転送
		spotLightData_->color = color_;
		spotLightData_->position = position_;
		spotLightData_->intensity = intensity_;
		spotLightData_->direction = direction_;
		spotLightData_->distance = distance_;
		spotLightData_->decay = decay_;
		spotLightData_->cosAngle = cosAngle_;
		spotLightData_->cosFalloffStart = cosFalloffStart_;
	}
}