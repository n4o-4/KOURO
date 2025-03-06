#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height; // uvStepSizeÇÃéZèo
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    
    output.color.rgb = float(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    return output;
}