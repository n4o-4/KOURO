#include "Particle.CS.hlsli"

struct TransformData
{
    float4x4 world;
};

ConstantBuffer<TransformData> gTransform : register(b0); // 0

// 既存パーティクル管理バッファ
RWStructuredBuffer<Particle> gParticles : register(u0); // 1
RWStructuredBuffer<uint> gFreeListIndex : register(u1); // 2
RWStructuredBuffer<uint> gFreeList : register(u2); // 3
ConstantBuffer<EmitterPoint> gEmitter : register(b1); // 4
ConstantBuffer<PerFrame> gPerFrame : register(b2); // 5
RWStructuredBuffer<uint> gNoiseUpdateList : register(u3); // 6
RWStructuredBuffer<uint> gBaseUpdateList : register(u4); // 7

[numthreads(512, 1, 1)]

void main(uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID)
{
    if (gEmitter.emit == 0) return; // 発射許可がない場合は処理しない
    
    if (DTid.x > gEmitter.count) return; // 発射数以上のスレッドは処理しない
    
    RandomGenerator generator;
    generator.seed = gPerFrame.time + (float) DTid.x * 13.0f;
    
    // FreeList から空きを取得
    uint freeListIndex;
    InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
    if (freeListIndex == 0)
    {
        InterlockedAdd(gFreeListIndex[0], 1);
        return;
    }

    uint particleIndex = gFreeList[freeListIndex - 1];

    // フラグを立てる
    gNoiseUpdateList[particleIndex] = 0;
    gBaseUpdateList[particleIndex] = 1;

    // スケール値をランダム補間
    float3 velocity = float3(
    lerp(gEmitter.velocityMin.x, gEmitter.velocityMax.x, generator.Generate1d()), // 横方向
    lerp(gEmitter.velocityMin.y, gEmitter.velocityMax.y, generator.Generate1d()), // 縦方向
    lerp(gEmitter.velocityMin.z, gEmitter.velocityMax.z, generator.Generate1d()) // 奥行き
    );

    // ---------------------------------------------
    // 2. ワールド行列から回転成分を取り出す
    // ---------------------------------------------
    // （上位3x3部分が回転＋スケールを含む）
    float3x3 worldRot =
    {
        gTransform.world[0].xyz, // X軸
    gTransform.world[1].xyz, // Y軸
    gTransform.world[2].xyz  // Z軸
    };

    // ---------------------------------------------
    // 3. ローカル速度をワールド空間に変換
    // ---------------------------------------------
    float3 worldVelocity = mul(velocity, worldRot); // float3 × float3x3
    
    // パーティクル初期化
    gParticles[particleIndex].translate = gEmitter.translate;
    gParticles[particleIndex].velocity = worldVelocity;
    gParticles[particleIndex].scale = float3(0.1f, 0.1f, 0.1f);
    gParticles[particleIndex].color = float4(1.0, 0.3265, 0.2908, 1.0f);
    gParticles[particleIndex].lifeTime = lerp(gEmitter.lifeTimeMin, gEmitter.lifeTimeMax, generator.Generate1d());
    gParticles[particleIndex].currentTime = gParticles[particleIndex].lifeTime;
}