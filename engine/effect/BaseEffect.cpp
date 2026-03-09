#include "BaseEffect.h"

namespace Kouro
{
	void BaseEffect::Initialize(GpuContext context, SrvManager* srvManager)
	{
		// DirectXCommonのポインタを保存
		dxCommon_ = DirectXCommon::GetInstance();

		// デバイスとコマンドリストのポインタを保存
		device_ = context.d3d12Context.device;
		commandList_ = context.d3d12Context.commandList;

		gpuResourceUtils_ = *context.gpuResourceUtils;

		srvManager_ = srvManager;

		// パイプラインの生成
		pipeline_ = std::make_unique<Pipeline>();
	}
}