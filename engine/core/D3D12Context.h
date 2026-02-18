#pragma once
#include <wrl.h>
#include <d3dx12.h>

namespace Kouro
{
	struct PipelineSet
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	};

	struct D3D12Context
	{
		ID3D12Device* device = nullptr;
		ID3D12GraphicsCommandList* commandList = nullptr;
	};
}