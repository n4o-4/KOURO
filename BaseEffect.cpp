#include "BaseEffect.h"

void BaseEffect::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	// 
	dxCommon_ = dxCommon;
	
	//
	srvManager_ = srvManager;

	// パイプラインの生成
	pipeline_ = std::make_unique<Pipeline>();
}
