#pragma once
#include <wrl.h>
#include <d3d12.h>

class GpuResourceUtils
{
public:

	GpuResourceUtils(ID3D12Device* device);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

private:

	ID3D12Device* device_ = nullptr;

};

