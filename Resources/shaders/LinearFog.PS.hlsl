#include "Fullscreen.hlsli"

struct Material
{
    float4x4 projectionInverse;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

Texture2D<float> gDepthTexture : register(t1);

SamplerState gSamplerPoint : register(s1);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f } },
    { { -1.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};


static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height; // uvStepSize�̎Z�o
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    float2 difference = float2(0.0f, 0.0f); // �c�����ꂼ��̏�ݍ��݂̌��ʂ��i�[����
    // �F���P�x�ɕϊ����āA��ݍ��݂��s���Ă����B����Filter�p��Kernel
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            // NDC -> View
            // gMaterial
            float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float viewZ = viewSpace.z * rcp(viewSpace.w); // �������W�n����f�J���g���W�n�֕ϊ�
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];
        }

    }
    
    float4 fogColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float fogStart = 10.0f;
    float fogEnd = 200.0f;
    
    float weight = length(difference * 0.45f);
    weight = saturate(weight);
    
    float3 baseColor = gTexture.Sample(gSampler, input.texcoord).rgb;
    float3 edgeColor = (1.0f - weight) * baseColor;
    
    // �t�H�O�v�Z
    float depth = gDepthTexture.Sample(gSamplerPoint, input.texcoord);
    float4 viewPos = mul(float4(0.0f, 0.0f, depth, 1.0f), gMaterial.projectionInverse);
    float viewDepth = viewPos.z / viewPos.w;
    
    float fogFactor = saturate(exp(-0.02f * (viewDepth - fogStart)));
    output.color.rgb = lerp(fogColor.rgb, edgeColor, fogFactor);
    output.color.a = 1.0f;
    
    return output;
}