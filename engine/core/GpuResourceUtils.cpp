#include "GpuResourceUtils.h"

#include "GpuResourceInternal.h"

namespace Kouro
{
	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::CreateBufferResource(size_t sizeInBytes)
	{
		// GpuResourceInternalのCreateBufferResourceを呼び出す
		return GpuResourceInternal::CreateBufferResource(device_, sizeInBytes);
	}

	Microsoft::WRL::ComPtr<ID3D12Resource> GpuResourceUtils::Create2DTextureResource(const DirectX::TexMetadata& metadata)
	{
		// GpuResourceInternalのCreate2DTextureResourceを呼び出す
		return GpuResourceInternal::Create2DTextureResource(device_, metadata);
	}
}