#include "Particle.CS.hlsli"

struct TransformData
{
    float4x4 world;
};

struct LineSegment
{
    float4 start;
    float4 end;
};

struct SegmentCount
{
    uint count;
};

StructuredBuffer<LineSegment> gLineSegments : register(t0); // 0
ConstantBuffer<TransformData> gTransform : register(b0);    // 1

// 既存パーティクル管理バッファ
RWStructuredBuffer<Particle> gParticles : register(u0);     // 2
RWStructuredBuffer<uint> gFreeListIndex : register(u1);     // 3
RWStructuredBuffer<uint> gFreeList : register(u2);          // 4
ConstantBuffer<EmitterSphere> gEmitter : register(b1);      // 5
ConstantBuffer<PerFrame> gPerFrame : register(b2);          // 6
ConstantBuffer<SegmentCount> gCount : register(b3);

[numthreads(512, 1, 1)]

void main(uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID)
{
    if (gEmitter.emit == 0) return;

    RandomGenerator generator;
    //generator.seed = asuint(gPerFrame.time) + DTid.x * 13;
    generator.seed = gPerFrame.time + (float)DTid.x * 13.0f;

    // StructuredBuffer からコピー
    const LineSegment lineSegment = gLineSegments[Gid.x];

    // 線分上のランダム位置
    float t = generator.Generate1d(); // 0〜1

    // float4 の xyz 成分を取り出して lerp
    float3 startPos = lineSegment.start.xyz;
    float3 endPos = lineSegment.end.xyz;
    float3 pos = lerp(startPos, endPos, t);

    // ワールド行列で変換
    pos = mul(float4(pos, 1.0f), gTransform.world).xyz;

    //float4x4 mat = gTransform.world;
    
    // FreeList から空きを取得
    uint freeListIndex;
    InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
    if (freeListIndex == 0)
    {
        InterlockedAdd(gFreeListIndex[0], 1);
        return;
    }

    uint particleIndex = gFreeList[freeListIndex - 1];

    // パーティクル初期化
    gParticles[particleIndex].translate = pos;
    gParticles[particleIndex].velocity.x = 0.0f;
    gParticles[particleIndex].velocity.y = generator.Generate1d() * 0.3f - 0.02f;
    gParticles[particleIndex].velocity.z = 0.0f;
    gParticles[particleIndex].scale = float3(0.1f, 0.1f, 0.1f);
    gParticles[particleIndex].color = float4(1.0, 0.3265, 0.2908,1.0f);
    gParticles[particleIndex].lifeTime = generator.Generate1d() * 2.0f;
    gParticles[particleIndex].currentTime = 0.0f;
}