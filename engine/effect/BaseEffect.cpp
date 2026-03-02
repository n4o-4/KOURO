#include "BaseEffect.h"

namespace Kouro
{
	void BaseEffect::Initialize(DirectXCommon* dxCommon, GpuContext* context)
	{
		dxCommon_ = dxCommon;
		cmdList_ = context->d3d12Context.commandList;
		srvManager_ = context->srvManager;
		utils_ = context->gpuResourceUtils;

		// パイプラインの生成
		pipeline_ = std::make_unique<Pipeline>();
	}

	void BaseEffect::SetCameraManager(CameraManager* cameraManager)
	{
	}

	void BaseEffect::Reset()
	{
	}
}