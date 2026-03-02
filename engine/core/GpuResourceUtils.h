#pragma once
#include <wrl.h>
#include <d3d12.h>

// external
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

namespace Kouro
{
	/// \brief GPUリソース作成ユーティリティクラス。

	class GpuResourceUtils
	{
	public: // 公開メンバ関数

		/**
		* \brief  コンストラクタ
		* \param  device : ID3D12Deviceのポインタ
		*/
		GpuResourceUtils(ID3D12Device* device) : device_(device) {}

		/**
		* \brief  バッファリソース作成関数
		* \param  sizeInBytes : バッファサイズ（バイト単位）
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

		/**
		* \brief  2Dテクスチャリソース作成関数
		* \param  metadata : テクスチャメタデータ
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> Create2DTextureResource(const DirectX::TexMetadata& metadata);

	private: // 非公開メンバ変数

		ID3D12Device* device_ = nullptr; //!< ID3D12Deviceのポインタ

	};
}