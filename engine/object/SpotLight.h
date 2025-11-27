#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"
#include "MyMath.h"

#include "DirectXCommon.h"

// \brief SpotLight  
// スポットライト（特定方向に円錐状に光を放つ光源）を管理するクラス。  
// 色・位置・方向・強度・距離・減衰・角度情報を設定し、局所的な照明や演出に使用される。

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
	void Initialize();

	void Update();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetSpotLightResource() { return spotLightResource_; }

public:

	Vector4 color_;
	Vector3 position_;
	float intensity_;
	Vector3 direction_;
	float distance_;
	float decay_;
	float cosAngle_;
	float cosFalloffStart_;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_;

	SpotLightData* spotLightData_ = nullptr;
};