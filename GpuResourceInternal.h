#pragma once
#include <wrl.h>
#include <d3d12.h>

namespace GpuResourceInternal
{
	/**
	* \brief  バッファリソース作成関数
	* \param  device      : ID3D12Deviceのポインタ
	* \param  sizeInBytes : バッファサイズ（バイト単位）
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
}