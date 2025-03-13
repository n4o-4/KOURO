#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);  
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    // maskの値が閾値以下の場合はdiscard
    if (mask < 0.3f)
    {
        discard;
    }
    
    float edge = 1.0f - smoothstep(0.3f, 0.33f, mask);
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edgeっぽい指定した色を加算
    output.color.rgb += edge * float3(1.0f, 0.4f, 0.3f);
    
    return output;
}