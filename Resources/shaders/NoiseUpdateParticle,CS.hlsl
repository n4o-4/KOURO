#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);
RWStructuredBuffer<uint> gNoiseUpdateList : register(u3);
ConstantBuffer<PerFrame> gPerFrame : register(b0);

[numthreads(1024, 1, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        if (gNoiseUpdateList[particleIndex] == 0) return;
        
        if (gParticles[particleIndex].color.a != 0)
        {   
            float3 noisePos = gParticles[particleIndex].translate * 0.9f + gPerFrame.time * 0.3f;
            float3 curl = CurlNoise3D(noisePos);
            gParticles[particleIndex].velocity += curl * 0.5f;
        }
    }
}