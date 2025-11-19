#include "GpuParticle.h"

void GpuParticle::Initialize()
{
	// DirectXCommon�̎擾
	dxCommon_ = DirectXCommon::GetInstance();

	resource_ = dxCommon_->CreateBufferResource(sizeof(ParticleCS) * 1024);
}