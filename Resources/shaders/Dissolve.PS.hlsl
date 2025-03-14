#include "Fullscreen.hlsli"

struct Material
{
    float Threshold; // 0.3f
    float ThresholdWidth; // 0.01f
   
    float3 edgeColor;
};

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);  
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    // mask�̒l��臒l�ȉ��̏ꍇ��discard
    if (mask < 0.3f)
    {
        discard;
    }
    
    float edge = 1.0f - smoothstep(gMaterial.Threshold, gMaterial.Threshold + gMaterial.ThresholdWidth, mask);
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edge���ۂ��w�肵���F�����Z
    output.color.rgb += edge * gMaterial.edgeColor;
    
    return output;
}