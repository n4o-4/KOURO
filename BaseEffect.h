#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "CameraManager.h"

struct Pipeline
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};


class BaseEffect
{
public:

	// ‰Šú‰»
	virtual void Initialize();

	// XV
	virtual void Update() = 0;

	// •`‰æ
	virtual void Draw() = 0;

private:

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;
};

