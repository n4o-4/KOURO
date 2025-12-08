#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);
RWStructuredBuffer<uint> gBaseUpdateList : register(u3);
ConstantBuffer<PerFrame> gPerFrame : register(b0);

[numthreads(1024, 1, 1)]

void main( uint3 DTid : SV_DispatchThreadID )
{
    uint particleIndex = DTid.x;
    if(particleIndex < kMaxParticles)
    {
        if (gBaseUpdateList[particleIndex] != 0) return;
        
        if (gParticles[particleIndex].color.a != 0)
        {
            gParticles[particleIndex].translate += gParticles[particleIndex].velocity * gPerFrame.deltaTime;
            gParticles[particleIndex].currentTime += gPerFrame.deltaTime;
            float alpha = 1.0f - (gParticles[particleIndex].currentTime / gParticles[particleIndex].lifeTime);
            gParticles[particleIndex].color.a = saturate(alpha);
        }
        
        if (gParticles[particleIndex].currentTime >= gParticles[particleIndex].lifeTime)
        {
            gParticles[particleIndex].scale = float3(0, 0, 0);
            uint freeListIndex;
            InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
            if (freeListIndex < kMaxParticles)
            {
                gFreeList[freeListIndex] = particleIndex;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], -1);
            }
        }
    }
}