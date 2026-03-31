#include "GpuResourceUtils.h"

#include "GpuResourceInternal.h"

namespace Kouro
{
	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateBufferResource(size_t sizeInBytes) const
	{
		// GpuResourceInternalのCreateBufferResourceを呼び出す
		return GpuResourceInternal::CreateBufferResource(device_, sizeInBytes);
	}
	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateUploadBufferResource(size_t sizeInBytes) const
	{
		// GpuResourceInternalのCreateUploadBufferResourceを呼び出す
		return GpuResourceInternal::CreateUploadBufferResource(device_, sizeInBytes);
	}
	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateDefaultBufferResource(size_t sizeInBytes) const
	{
		// GpuResourceInternalのCreateDefaultBufferResourceを呼び出す
		return GpuResourceInternal::CreateDefaultBufferResource(device_, sizeInBytes);
	}
}