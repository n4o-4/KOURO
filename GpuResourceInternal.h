#pragma once
#include <wrl.h>
#include <d3d12.h>

namespace GpuResourceInternal
{
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
}