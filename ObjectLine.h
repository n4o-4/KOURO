#pragma once
#include "LineObjectManager.h"
#include "LineDrawerBase.h"
#include "LineModel.h"

struct LineForGPU
{
	Matrix4x4 matWorld;
	Vector4 color;
};

struct ScanEffectCB
{
	bool enableScan; // スキャンエフェクトを有効にするかどうか
	bool isRenderScanned; // スキャン部分を描画するかどうか
	float progress;   // 0.0f～1.0f スキャンの進行度
	float thickness;  // スキャンの厚み
};

class ObjectLine
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lineDrawer"></param>
	void Initialize(LineModel* model);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldTransform"></param>

	void Draw(WorldTransform* worldTransform);

	void SetColor(const Vector4& color)
	{
		lineData_->color = color;
	}

private:

	LineModel* model_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> lineResource_ = nullptr;
	LineForGPU* lineData_ = nullptr;

	ScanEffectCB* scanEffectData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> scanEffectResource_ = nullptr;
};

