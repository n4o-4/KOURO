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
    if (gEmitter.emit == 0) return;
    
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
    
    float3 forward = (gTransform.world[2]).xyz; // ワールド行列のZ軸を前方とする
    float3 baseDir = -normalize(forward); // 後ろ向き

    // 角度パラメータ（中心からどれくらい開くか）
    float spreadAngle = radians(10.0f);

    // velocityRange の使い方を拡張
    float3 coneScaleMin = gEmitter.velocityRange[0];
    float3 coneScaleMax = gEmitter.velocityRange[1];

    // スケール値をランダム補間
    float3 coneScale = float3(
    lerp(coneScaleMin.x, coneScaleMax.x, generator.Generate1d()), // 横方向
    lerp(coneScaleMin.y, coneScaleMax.y, generator.Generate1d()), // 縦方向
    lerp(coneScaleMin.z, coneScaleMax.z, generator.Generate1d()) // 奥行き
    );

    // ---------------------------------------------
    // 1. ランダム方向を生成（円錐形状）
    // ---------------------------------------------
    float u = generator.Generate1d();
    float v = generator.Generate1d();
    float theta = acos(lerp(cos(spreadAngle), 1.0f, u));
    float phi = v * 2.0f * 3.141592f;

    // 円錐内の単位ベクトル（ローカル座標）
    float3 localDir = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

    // ---------------------------------------------
    // 2. 形状スケールを反映（直径調整）
    // ---------------------------------------------
    localDir *= coneScale; // 各軸ごとに伸縮

    // ---------------------------------------------
    // 3. baseDir に合わせてワールド方向へ変換
    // ---------------------------------------------
    float3 up = abs(baseDir.y) < 0.95f ? float3(0, 1, 0) : float3(1, 0, 0);
    float3 right = normalize(cross(up, baseDir));
    float3 newUp = cross(baseDir, right);

    // ローカル空間 → ワールド空間
    float3 worldDir = normalize(localDir.x * right + localDir.y * newUp + localDir.z * baseDir);

    // ---------------------------------------------
    // 4. 距離（Z方向スケール）で速度を決定
    // ---------------------------------------------
    float speed = lerp(coneScaleMin.z, coneScaleMax.z, generator.Generate1d());
    gParticles[particleIndex].velocity = worldDir * speed;
    
    // パーティクル初期化
    gParticles[particleIndex].translate = gEmitter.translate;
    gParticles[particleIndex].scale = float3(0.1f, 0.1f, 0.1f);
    gParticles[particleIndex].color = float4(1.0, 0.3265, 0.2908, 1.0f);
    gParticles[particleIndex].lifeTime = lerp(gEmitter.lifeTimeRange[0], gEmitter.lifeTimeRange[1], generator.Generate1d());
    gParticles[particleIndex].currentTime = gParticles[particleIndex].lifeTime;
}