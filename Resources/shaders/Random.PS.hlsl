#include "Fullscreen.hlsli"
#include "Random.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct Material
{
    float time;
};

ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // —”¶¬
    float random = rand2dTo1d(input.texcoord * gMaterial.time);
    
    output.color = float4(random, random, random, 1.0f);
    
    return output;
}