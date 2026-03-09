#pragma once
#include <d3d12.h>

#include "GpuResourceUtils.h"

namespace Kouro
{
	template <typename T>
	class UploadBuffer
	{
	public:

		static constexpr SIZE_T kDefaultElementCount = 256; //!< デフォルトのバッファ数

	public:

		/**
		* \brief  コンストラクタ
		* \param desc.elementCount : バッファ数
		*/
		UploadBuffer(const SIZE_T elementCount) : elementCount_(elementCount) {};

		/**
		* \brief  初期化
		* \param  device : デバイス
		*/
		void Initialize(GpuResourceUtils* utils)
		{
			// utilsがnullptrでないことを確認
			assert(utils != nullptr);

			// バッファのサイズを計算
			const SIZE_T bufferSize = sizeof(T) * elementCount_;

			// アップロードバッファの作成
			resource_ = utils->CreateUploadBufferResource(bufferSize);

			// マップ
			resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
		}


	private:

		const SIZE_T elementCount_; //!< 

		Microsoft::WRL::ComPtr<ID3D12Resource> resource_; //!< アップロードバッファのリソース
		T* mappedData_ = nullptr; //!< マップされたデータのポインタ
	};
}