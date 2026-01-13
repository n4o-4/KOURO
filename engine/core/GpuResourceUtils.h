#pragma once
#include <wrl.h>
#include <d3d12.h>

class GpuResourceUtils
{
public:

	/**
	* \brief  コンストラクタ
	* \param  device : ID3D12Deviceのポインタ
	*/
	GpuResourceUtils(ID3D12Device* device);

	/**
	* \brief  バッファリソース作成関数
	* \param  sizeInBytes : バッファサイズ（バイト単位）
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

private:

	ID3D12Device* device_ = nullptr; //!< ID3D12Deviceのポインタ

};

