#include "object3d.hlsli"

struct ViewProjection
{
    float3 worldPosition;    // ���[���h��Ԃł̃J�����ʒu
    float4x4 ViewProjection; // �r���[���e�s��
};

struct WorldMatrix
{
    float4x4 World;                // ���[���h�s��
    float4x4 WorldInversTranspose; // ���[���h�s��̋t�]�u�s��
};

ConstantBuffer<ViewProjection> gViewProjection : register(b0);

ConstantBuffer<WorldMatrix> gWorldMatrix : register(b1);

struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4x4 WVP = mul(gWorldMatrix.World, gViewProjection.ViewProjection);
    output.position = mul(input.position, WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gWorldMatrix.WorldInversTranspose));
    output.worldPosition = mul(input.position, gWorldMatrix.World).xyz;
    output.cameraPosition = gViewProjection.worldPosition;
    return output;
}