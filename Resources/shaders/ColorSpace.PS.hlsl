#include "Fullscreen.hlsli"

struct Material
{
    float hue;
    float saturation;
    float value;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float3 HSVToRGB(float3 hsv)
{
    float3 rgb = float3(0, 0, 0);
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;
    int i = (int) (h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    i %= 6;
    if (i == 0)
        rgb = float3(v, t, p);
    else if (i == 1)
        rgb = float3(q, v, p);
    else if (i == 2)
        rgb = float3(p, v, t);
    else if (i == 3)
        rgb = float3(p, q, v);
    else if (i == 4)
        rgb = float3(t, p, v);
    else if (i == 5)
        rgb = float3(v, p, q);
    return rgb;
}

float3 RGBToHSV(float3 rgb)
{
    float3 hsv = float3(0, 0, 0);
    float maxVal = max(rgb.r, max(rgb.g, rgb.b));
    float minVal = min(rgb.r, min(rgb.g, rgb.b));
    hsv.z = maxVal;
    if (maxVal > 0)
    {
        hsv.y = (maxVal - minVal) / maxVal;
        if (hsv.y > 0)
        {
            if (maxVal == rgb.r)
                hsv.x = (rgb.g - rgb.b) / (maxVal - minVal);
            else if (maxVal == rgb.g)
                hsv.x = 2 + (rgb.b - rgb.r) / (maxVal - minVal);
            else
                hsv.x = 4 + (rgb.r - rgb.g) / (maxVal - minVal);
            hsv.x /= 6;
            if (hsv.x < 0)
                hsv.x += 1;
        }
    }
    return hsv;
}

float WrapValue(float value, float minRange, float maxRange)
{
    float range = maxRange - minRange;
    float modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float3 hsv = RGBToHSV(textureColor.rgb);
    
    hsv.x += gMaterial.hue;
    hsv.y += gMaterial.saturation;
    hsv.z += gMaterial.value;
    
    // ”’l‚ª0~1‚Ì”ÍˆÍ‚ÉŽû‚Ü‚é‚æ‚¤‚É‚·‚é
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    
    output.color.rgb = HSVToRGB(hsv);
    output.color.a = textureColor.a;
    
    return output;
}