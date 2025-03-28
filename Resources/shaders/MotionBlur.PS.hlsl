#include "Fullscreen.hlsli"

struct Material
{
    int numSamples;
    float2 center;
    float blurWidth;
    float3 diff;
};
   
Texture2D<float4> gTexture : register(t0);
SamplerState gSamplerLinear : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{ 
    // `diff` を動きの方向と強さとして利用
    // `diff` は前フレームと現在フレームの差分で、カメラや物体の動きを示すベクトル
    float2 direction = gMaterial.diff.xz; // x, y の差分を移動方向に使用

    // 色の初期化
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);

    // 動きの方向に沿って複数のサンプルを取る
    for (int sampleIndex = 0; sampleIndex < gMaterial.numSamples; ++sampleIndex)
    {
        // 現在のテクスチャ座標に、動きの方向に沿ってサンプル点を進める
        float2 texcoord = input.texcoord + direction * gMaterial.blurWidth * float(sampleIndex);

        // サンプルを取る
        outputColor.rgb += gTexture.Sample(gSamplerLinear, texcoord).rgb;
    }

    // サンプルの平均化
    outputColor.rgb *= rcp(gMaterial.numSamples);

    // 出力色を設定
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f; // 不透明度を1に設定

    return output;
}