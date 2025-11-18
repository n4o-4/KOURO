#include "Fullscreen.hlsli"


struct Material
{
    float4 glowColor; // 発光カラー（例: float3(1.0, 1.0, 1.0)）
    float2 center; // 吸い込み中心（例: float2(0.5, 0.5)）
    float time; // 0.0 → 1.0
    float intensity; // 吸い込みの強さ

    float glowPower; // 発光の強さ
    float padding[3];
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float2 dir = gMaterial.center - input.texcoord;
    float4 col = float4(0, 0, 0, 0);

    int samples = 8; // サンプリング回数
    for (int i = 0; i < samples; ++i)
    {
        float t = (i / (float) samples) * gMaterial.time * gMaterial.intensity;
        float2 uv = input.texcoord + dir * t;
        col += gTexture.Sample(gSampler, saturate(uv));
    }
    col /= samples;

    // 発光も加える
    float glow = smoothstep(0.0, 1.0, gMaterial.time) * gMaterial.glowPower;
    col.rgb += glow * gMaterial.glowColor.rgb;

    output.color = col;
    return output;
}