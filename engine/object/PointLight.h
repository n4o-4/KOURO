#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

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
	* \brief  ポイントライトのresourceを取得する
	* \return pointLightResource
	*/
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetPointLightResource() { return pointLightResource_; }

public:

	Vector4 color_;
	Vector3 position_;
	float intensity_;
	float radius_;
	float decay_;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource_;

	PointLightData* pointLightData_ = nullptr;
};