#pragma once
#include "D3D12Context.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "GpuResourceUtils.h"

namespace Kouro
{
	struct GpuContext
	{
		D3D12Context d3d12Context;
		SrvManager* srvManager = nullptr;
		UavManager* uavManager = nullptr;
		GpuResourceUtils* gpuResourceUtils = nullptr;
	};
}