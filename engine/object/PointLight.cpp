#include "PointLight.h"
#include "InitialLightSetting.h"

void PointLight::Initilize()
{
	// bufferResourceの生成
	pointLightResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(PointLightData));

	// データをマップ
	pointLightResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	// 数値を初期化
	color_ = LightDefault::kDefaultColor;
	position_ = LightDefault::PointLight::kDefaultPosition;
	intensity_ = LightDefault::kDefaultIntensity;
	radius_ = LightDefault::PointLight::kDefaultRadius;
	decay_ = LightDefault::PointLight::kDefaultDecay;

	// 数値をデータに代入
	pointLightData_->color = color_;
	pointLightData_->position = position_;
	pointLightData_->intensity = intensity_;
	pointLightData_->radius = radius_;
	pointLightData_->decay = decay_;
}

void PointLight::Update()
{

	// 数値をデータに代入
	pointLightData_->color = color_;
	pointLightData_->position = position_;
	pointLightData_->intensity = intensity_;
	pointLightData_->radius = radius_;
	pointLightData_->decay = decay_;

}
