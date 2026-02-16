#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<int> gFreeList : register(u2);
ConstantBuffer<EmitterSphere> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if(gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = dot(DTid, uint3(1, 738, 193)) + gPerFrame.time;
        
        for (uint countIndex = 0; countIndex < gEmitter.count;countIndex++)
        {     
            uint freeListIndex;
            InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);

            if (freeListIndex > 0)
            {
                uint particleIndex = gFreeList[freeListIndex - 1];
                gParticles[particleIndex].scale = float3(1, 1, 1);
                gParticles[particleIndex].translate = generator.Generate3d() * 11 - 6;
                gParticles[particleIndex].velocity = generator.Generate3d() * 0.05 - 0.03;
                gParticles[particleIndex].color.rgb = generator.Generate3d();
                gParticles[particleIndex].color.a = 1.0f;
                gParticles[particleIndex].currentTime = 0.0f;
                gParticles[particleIndex].lifeTime = 5.0f;
                gParticles[particleIndex].currentTime = gParticles[particleIndex].lifeTime;
            }
            else
            {
                InterlockedAdd(gFreeListIndex[0], 1);
            }
        }
    }
}