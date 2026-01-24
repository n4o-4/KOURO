#include "BaseEffect.h"

namespace Kouro
{
	void BaseEffect::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
	{
		// 
		dxCommon_ = dxCommon;

		//
		srvManager_ = srvManager;

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