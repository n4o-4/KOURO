#pragma once
#include "DirectXCommon.h"
#include "Vectors.h"

struct ParticleCS
{
	Vector3 translate;
	Vector3 scale;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

class GpuParticle
{
public:

	void Initialize();


private:

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_ = nullptr;

};

