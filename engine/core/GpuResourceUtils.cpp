#include "GpuResourceUtils.h"

#include "GpuResourceInternal.h"

namespace Kouro
{
	GpuResourceUtils::GpuResourceUtils(ID3D12Device* device)
	{
		device_ = device;
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateBufferResource(size_t sizeInBytes)
	{
		// GpuResourceInternalのCreateBufferResourceを呼び出す
		return GpuResourceInternal::CreateBufferResource(device_, sizeInBytes);
	}
}