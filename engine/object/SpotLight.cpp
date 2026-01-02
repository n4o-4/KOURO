#include "SpotLight.h"
#include "SpotLightDefaults.h"

void SpotLight::Initialize()
{
	// bufferResourceの生成
	spotLightResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(SpotLightData));

	// データをマップ
	spotLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	
	// 初期値設定	
	color_ = SpotLightDefaults::kDefaultColor;
	position_ = SpotLightDefaults::kDefaultPosition;
	intensity_ = SpotLightDefaults::kDefaultIntensity;
	direction_ = Normalize(SpotLightDefaults::kDefaultDirection);
	distance_ = SpotLightDefaults::kDefaultDistance;
	decay_ = SpotLightDefaults::kDefaultDecay;
	cosAngle_ = std::cos(SpotLightDefaults::kDefaultAngle);
	cosFalloffStart_ = SpotLightDefaults::kDefaultCosFalloffStart;

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
