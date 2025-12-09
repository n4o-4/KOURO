#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);
RWStructuredBuffer<uint> gBaseUpdateList : register(u3);
RWStructuredBuffer<uint> gNoiseUpdateList : register(u4);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        if (particleIndex == 0)
        {
            gFreeListIndex[0] = kMaxParticles; // 個数を入れる（例: 1024）
        }
        gParticles[particleIndex] = (Particle) 0;
        gFreeList[particleIndex] = particleIndex;
        gBaseUpdateList[particleIndex] = 1; // すべてのパーティクルは更新を有効かする
        gNoiseUpdateList[particleIndex] = 0; // いったん無効にしておく
    }
}