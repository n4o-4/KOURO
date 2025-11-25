#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeCounter : register(u1);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        if (particleIndex == 0)
        {
            gFreeCounter[0] = 0;
        }
        
        // パーティクルを初期化
            gParticles[particleIndex] = (Particle) 0;
        gParticles[particleIndex].scale = float3(0.5f,0.5f,0.5f);
        gParticles[particleIndex].color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}