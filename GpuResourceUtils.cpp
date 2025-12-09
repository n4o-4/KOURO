#include "GpuResourceUtils.h"

#include "GpuResourceInternal.h"

GpuResourceUtils::GpuResourceUtils(ID3D12Device* device)
{
	device_ = device;
}

Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateBufferResource(size_t sizeInBytes)
{
	return GpuResourceInternal::CreateBufferResource(device_, sizeInBytes);
}