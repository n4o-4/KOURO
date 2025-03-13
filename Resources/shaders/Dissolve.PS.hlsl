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
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    // mask‚Ì’l‚ªè‡’lˆÈ‰º‚Ìê‡‚Ídiscard
    if (mask < 0.3f)
    {
        discard;
    }
    
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    return output;
}