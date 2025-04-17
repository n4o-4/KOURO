#include "object3d.hlsli"

struct ViewProjection
{
    float3 worldPosition;
    float4x4 ViewProjection;
};

struct WorldMatrix
{
    float4x4 World;
    float4x4 WorldInversTranspose;
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
    float4x4 WVP = mul(gWorldMatrix.World, gViewProjection.ViewProjection); // âûã}èàíu
    output.position = mul(input.position, WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gWorldMatrix.WorldInversTranspose));
    output.worldPosition = mul(input.position, gWorldMatrix.World).xyz;
    output.cameraPosition = gViewProjection.worldPosition;
    return output;
}