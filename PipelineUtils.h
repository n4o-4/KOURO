#pragma once
#include <memory>
#include <wrl.h>
#include <d3d12.h>

namespace Kouro
{
	class PipelineUtils
	{
	public: // 公開メンバ関数

		/**
		* \brief  コンストラクタ
		* \param  device : ID3D12Deviceのポインタ
		*/
		PipelineUtils(const ID3D12Device* device) : device_(device) {};


	private: // 非公開メンバ変数

		const ID3D12Device* device_ = nullptr; //!< ID3D12Deviceのポインタ

	};
}