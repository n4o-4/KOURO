#pragma once
#include <cstdint>
#include <d3d12.h>

/// \brief アロケーション構造体
struct Allocator
{
	void* cpuPointer; //!< CPU側の汎用ポインタ
	D3D12_GPU_VIRTUAL_ADDRESS gpuAddress; //!< GPU側の仮想アドレス
};

namespace Kouro
{
	/// \brief アップロードリングバッファクラス
	class UploadRingBuffer
	{
	public:



	private:

		uint8_t* mappedData_ = nullptr; //!< マップされたデータのポインタ

	};
}