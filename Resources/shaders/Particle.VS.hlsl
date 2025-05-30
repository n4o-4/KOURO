#include "Particle.hlsli"

struct ParticleForGPU
{
    float3 position;
    float _pad0;
    float3 scale;
    float _pad1;
    float3 velocity;
    float _pad2;
    float4 color;
    float stretch;
    float3 _pad3;
   
};

struct CameraForGPU
{
    float4x4 view;
    
    float4x4 projection;
    
    float4x4 billboardMatrix;
    
    float3 cameraPosition;
};

struct FlagsForGPU
{
    int enableBillboard;
    int enbaleStretch;
};

StructuredBuffer<ParticleForGPU> gParticle : register(t0);

ConstantBuffer<CameraForGPU> gCamera : register(b0);

ConstantBuffer<FlagsForGPU> gFlags : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

//VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
//{
//    //VertexShaderOutput output;
//    //output.position = mul(input.position, gParticle[instanceId].WVP);
//    //output.texcoord = input.texcoord;
//    //output.normal = normalize(mul(input.normal, (float3x3) gParticle[instanceId].World));
//    //output.color = gParticle[instanceId].color;
//    //return output;
    
//    VertexShaderOutput output;

//    ParticleForGPU particle = gParticle[instanceId];

//    // パーティクルのワールド位置
//    float3 centerPos = particle.position;

//    // カメラの右方向ベクトル（ビュー行列の0列目）
//    float3 right = normalize(float3(gCamera.view[0][0], gCamera.view[1][0], gCamera.view[2][0]));
//    //float3 right = -normalize(float3(gCamera.view[0][0], gCamera.view[1][0], gCamera.view[2][0]));

//    // カメラの上方向ベクトル（ビュー行列の1列目）
//    float3 up = normalize(float3(gCamera.view[0][1], gCamera.view[1][1], gCamera.view[2][1]));

//    // 速度方向ベクトル（伸縮方向）
//    float3 velocityDir = float3(0, 1, 0); // 仮の初期化

//    float speed = length(particle.velocity);
//    if (speed < 1e-5)
//    {
//        velocityDir = normalize(centerPos - gCamera.cameraPosition); // fallback
//    }
//    else
//    {
//        velocityDir = normalize(particle.velocity);
//    }
    
//    // 速度がほぼゼロなら代替ベクトル（例えばカメラ正面方向）を使う
//    if (length(particle.velocity) < 1e-5)
//    {
//        velocityDir = normalize(centerPos - gCamera.cameraPosition);
//    }

//    // 伸縮倍率の計算（例：baseスケール + 速度に比例した伸縮）
//    float stretchScale = particle.scale.x; // 横幅ベーススケール（例）
//    float stretchLength = particle.scale.y + length(particle.velocity) * particle.stretch;

//    // input.position.xy は板ポリのローカル座標(-0.5～+0.5など)
//    // 横方向は right ベクトル、縦方向は velocityDir に沿わせて伸縮

//    // 横方向オフセット
//    float3 offsetRight = right * input.position.x * stretchScale;

//    // 縦方向オフセット（ストレッチ方向）
//    float3 offsetStretch = velocityDir * input.position.y * stretchLength;

//    // 最終的な頂点位置（ワールド座標）
//    float3 worldPos = centerPos + offsetRight + offsetStretch;

//    // ワールド→ビュー→射影変換
//    float4 viewPos = mul(float4(worldPos, 1.0f), gCamera.view);
//    float4 projPos = mul(viewPos, gCamera.projection);

//    output.position = projPos;
//    output.texcoord = input.texcoord;
//    output.color = particle.color;

//    return output;
//}

//VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
//{
//    VertexShaderOutput output;

//    // パーティクルのインスタンスデータ
//    ParticleForGPU particle = gParticle[instanceId];

//    // パーティクルのワールド中心座標
//    float3 centerPos = particle.position;

//    // カメラの右方向ベクトル（ビュー行列の0列目）
//    float3 right = normalize(float3(
//        gCamera.view[0][0],
//        gCamera.view[1][0],
//        gCamera.view[2][0]
//    ));

//    // カメラの上方向ベクトル（ビュー行列の1列目）
//    float3 up = normalize(float3(
//        gCamera.view[0][1],
//        gCamera.view[1][1],
//        gCamera.view[2][1]
//    ));

//    // 速度ベクトルの正規化（ストレッチ方向）
//    float speed = length(particle.velocity);
//    float3 velocityDir = (speed < 1e-5)
//        ? normalize(centerPos - gCamera.cameraPosition) // fallback: カメラ方向
//        : normalize(particle.velocity);

//    // 伸縮スケール
//    float stretchScale = particle.scale.x; // 横幅スケール
//    float stretchLength = particle.scale.y + speed * particle.stretch; // 長さは速度に比例

//    // ローカル座標 (-0.5〜+0.5) を使って offset 計算
//    float3 offsetRight = right * input.position.x * stretchScale;
//    float3 offsetStretch = velocityDir * input.position.y * stretchLength;

//    // 最終的なワールド座標（position を2重に足さない！）
//    float3 worldPos = centerPos + offsetRight + offsetStretch;

//    // ワールド → ビュー → 射影変換
//    float4 viewPos = mul(float4(worldPos, 1.0f), gCamera.view);
//    float4 projPos = mul(viewPos, gCamera.projection);

//    output.position = projPos;
//    output.texcoord = input.texcoord;
//    output.color = particle.color;

//    return output;
//}

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;

    ParticleForGPU particle = gParticle[instanceId];
    float3 centerPos = particle.position;
    float3 worldPos;

    // velocity
    float speed = length(particle.velocity);
    float3 velocityDir = (speed < 1e-5)
        ? normalize(centerPos - gCamera.cameraPosition)
        : normalize(particle.velocity);

    // 共通スケール
    float stretchWidth = particle.scale.x;
    float stretchLength = particle.scale.y + speed * particle.stretch;

    // カメラ前方向
    float3 cameraForward = normalize(-float3(
        gCamera.view[0][2],
        gCamera.view[1][2],
        gCamera.view[2][2]
    ));

    // カメラの横方向
    float3 camRight = normalize(float3(
        gCamera.view[0][0],
        gCamera.view[1][0],
        gCamera.view[2][0]
    ));

    // カメラの上方向
    float3 camUp = normalize(float3(
        gCamera.view[0][1],
        gCamera.view[1][1],
        gCamera.view[2][1]
    ));

    if (gFlags.enableBillboard == 0 && gFlags.enbaleStretch == 0)
    {
        // 通常スプライト（回転も伸縮も無し）
        float3 offset = camRight * input.position.x * stretchWidth + camUp * input.position.y * particle.scale.y;
        worldPos = centerPos + offset;
    }
    else if (gFlags.enableBillboard == 1 && gFlags.enbaleStretch == 0)
    {
        // 通常のビルボード（向きだけカメラを向く）
        float3 offset = camRight * input.position.x * stretchWidth + camUp * input.position.y * particle.scale.y;
        worldPos = centerPos + offset;
    }
    else if (gFlags.enableBillboard == 0 && gFlags.enbaleStretch == 1)
    {
        // 回転は無し、ストレッチのみ（速度方向に伸びる）
        float3 offsetRight = camRight * input.position.x * stretchWidth;
        float3 offsetStretch = velocityDir * input.position.y * stretchLength;
        worldPos = centerPos + offsetRight + offsetStretch;
    }
    else if (gFlags.enableBillboard == 1 && gFlags.enbaleStretch == 1)
    {
        // ビルボード + ストレッチ（velocity方向に伸びるがカメラ正面）
        float3 right = normalize(cross(cameraForward, velocityDir));
        float3 offset = right * input.position.x * stretchWidth + velocityDir * input.position.y * stretchLength;
        worldPos = centerPos + offset;
    }

    // ビュー・射影変換
    float4 viewPos = mul(float4(worldPos, 1.0f), gCamera.view);
    float4 projPos = mul(viewPos, gCamera.projection);

    output.position = projPos;
    output.texcoord = input.texcoord;
    output.color = particle.color;

    return output;
}