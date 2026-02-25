#pragma once
#include <unordered_map>

#include "GpuContext.h"

/**
* \brief  複数のレンダーターゲットを管理するクラス
*/

namespace Kouro
{
	class RenderTargetManager
	{
	private:

		/**
		* \brief レンダーターゲットの構造体
		*/
		struct RenderTarget
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr; //!< レンダリングターゲットのリソース
			D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = {}; //!< シェーダーリソースビューのGPUハンドル
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {}; //!< レンダリングターゲットビューのCPUハンドル
			D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {}; //!< 深度ステンシルビューのCPUハンドル
		};

	public:

		
		RenderTargetManager()

	private:

		GpuContext context_; //!< GPUコンテキスト

		std::unordered_map<std::string, RenderTarget> renderTargets_; //!< レンダーターゲットのマップ
	};
}