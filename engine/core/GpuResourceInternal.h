#pragma once
#include <wrl.h>
#include <d3d12.h>

/// external
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

namespace Kouro
{
	namespace GpuResourceInternal
	{
		/**
		* \brief  バッファリソース作成関数
		* \param  device      : ID3D12Deviceのポインタ
		* \param  sizeInBytes : バッファサイズ（バイト単位）
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

		/**
		* \brief  2Dテクスチャリソース作成関数
		* \param  device : ID3D12Deviceのポインタ
		* \param  metadata : テクスチャメタデータ
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> Create2DTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	}
}
