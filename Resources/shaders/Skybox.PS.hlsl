#include "Skybox.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
    float3 specularColor;
};

struct DirectionLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float4 color; // ���C�g�̐F
    float3 position; // ���C�g�̐F
    float intensity; // �P�x
    float radius; // ���C�g�̓͂��ő勗��
    float decay; // ������
};

struct SpotLight
{
    float4 color; // ���C�g�̐F
    float3 position; // ���C�g�̈ʒu
    float intensity; // �P�x
    float3 direction; // ���C�g�̕���
    float distance; // ���C�g�̓͂��ő勗��
    float decay; // ������
    float cosAngle; // �X�|�b�g���C�g�̗]��
    float cosFalloffStart;
};


struct Direction
{
    float3 direction;
};


ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionLight> gDirectionalLight : register(b1);

ConstantBuffer<PointLight> gPointLight : register(b2);

ConstantBuffer<SpotLight> gSpotLight : register(b3);

ConstantBuffer<Direction> gDirection : register(b4);

TextureCube<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSampler,input.texcoord);
    
    output.color = textureColor * gMaterial.color;
    
    return output;
}