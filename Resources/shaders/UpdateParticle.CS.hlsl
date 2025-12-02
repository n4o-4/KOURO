#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);
ConstantBuffer<PerFrame> gPerFrame : register(b0);

[numthreads(1024, 1, 1)]

void main( uint3 DTid : SV_DispatchThreadID )
{
    uint particleIndex = DTid.x;
    if(particleIndex < kMaxParticles)
    {
        if(gParticles[particleIndex].color.a != 0)
        {
            //float3 noise = PerlinNoise3D(gParticles[particleIndex].translate * 0.2f + gPerFrame.time * 0.5f);
            //gParticles[particleIndex].velocity += noise * 0.0001f;
            
            float3 noisePos = gParticles[particleIndex].translate * 0.9f + gPerFrame.time * 0.3f;
            float3 curl = CurlNoise3D(noisePos);
            gParticles[particleIndex].velocity += curl * 0.5f;
            
            
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