#include "DirectionalLight.h"
#include "InitialLightSetting.h"

void DirectionalLight::Initialize()
{
	// bufferResourceの生成
	directionalLightResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(DirectionalLightData));

	// データをマップ
	directionalLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	// 数値を初期化
	color_ = LightDefault::kDefaultColor;
	direction_ = LightDefault::DirectionalLight::kDefaultDirection;
	intensity_ = LightDefault::kDefaultIntensity;
	
	directionalLightData_->color = color_;
	directionalLightData_->direction = direction_;
	directionalLightData_->intensity = intensity_;
}

void DirectionalLight::Update()
{
	// データ転送
	directionalLightData_->color = color_;
	directionalLightData_->direction = direction_;
	directionalLightData_->intensity = intensity_;
}
