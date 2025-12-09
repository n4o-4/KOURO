#include "Particle.CS.hlsli"

struct TransformData
{
    float4x4 world;
};

struct EmitterSphereShell
{
    float3 translate;
    float minRadius;
    float maxRadius;
    
    uint emit;
};

RWStructuredBuffer<Particle> gParticles : register(u0);     // 0
RWStructuredBuffer<uint> gFreeListIndex : register(u1);     // 1
RWStructuredBuffer<uint> gFreeList : register(u2);          // 2

ConstantBuffer<EmitterSphereShell> gEmitter : register(b0); // 3
ConstantBuffer<PerFrame> gPerFrame : register(b1);          // 4

RWStructuredBuffer<uint> gNoiseUpdateList : register(u3);   // 5
RWStructuredBuffer<uint> gBaseUpdateList : register(u4);    // 6

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (gEmitter.emit == 0)
        return;

    RandomGenerator generator;
    generator.seed = gPerFrame.time + (float) DTid.x * 31.0f;

    //---------------------------------------------------
    // ① ランダム単位ベクトルを作る
    //---------------------------------------------------
    float3 dir;
    {
        float3 v;
        v.x = generator.Generate1d() * 2.0f - 1.0f;
        v.y = generator.Generate1d() * 2.0f - 1.0f;
        v.z = generator.Generate1d() * 2.0f - 1.0f;

        // 0 ベクトルにならないように
        if (dot(v, v) < 0.0001f)
        {
            v = float3(0.0f, 1.0f, 0.0f);
        }

        dir = normalize(v);
    }

    //---------------------------------------------------
    // ② minRadius ～ maxRadius のランダム半径
    //---------------------------------------------------
    float r = lerp(gEmitter.minRadius, gEmitter.maxRadius, generator.Generate1d());

    //---------------------------------------------------
    // ③ 発生位置 = center + dir * r
    //---------------------------------------------------
    float3 pos = gEmitter.translate + dir * r;

    //---------------------------------------------------
    // ④ FreeList から空きを1つ取る
    //---------------------------------------------------
    uint freeListIndex;
    InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
    if (freeListIndex == 0)
    {
        InterlockedAdd(gFreeListIndex[0], 1);
        return;
    }

    uint particleIndex = gFreeList[freeListIndex - 1];

    //---------------------------------------------------
    // ⑤ パーティクル初期化
    //---------------------------------------------------
    gNoiseUpdateList[particleIndex] = 0;
    gBaseUpdateList[particleIndex] = 0;

    gParticles[particleIndex].translate = pos;

    gParticles[particleIndex].velocity.x = 0.0f;
    gParticles[particleIndex].velocity.y = 0.0f/*generator.Generate1d() * 0.3f - 0.02f*/;
    gParticles[particleIndex].velocity.z = 0.0f;

    //gParticles[particleIndex].scale = float3(0.1f, 0.1f, 0.1f);
    gParticles[particleIndex].scale = generator.Generate3d() * 200.0f;
    gParticles[particleIndex].color.xyz = generator.Generate3d();
    gParticles[particleIndex].color.a = 1.0f;
    gParticles[particleIndex].lifeTime = 1;
    gParticles[particleIndex].currentTime = 0.0f;
}
