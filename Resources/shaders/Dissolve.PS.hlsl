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
    
    // mask�̒l��臒l�ȉ��̏ꍇ��discard
    if (mask < 0.3f)
    {
        discard;
    }
    
    float edge = 1.0f - smoothstep(0.3f, 0.33f, mask);
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edge���ۂ��w�肵���F�����Z
    output.color.rgb += edge * float3(1.0f, 0.4f, 0.3f);
    
    return output;
}