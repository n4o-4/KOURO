struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS; 
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

struct Material
{
    float Time;
    float4x4 ViewProj;
};

ConstantBuffer<Material> gMaterial : register(b0);

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
    HS_CONSTANT_DATA_OUTPUT input,
    const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch,
    float3 barycentricCoords : SV_DomainLocation)
{
    float3 p0 = patch[0].vPosition;
    
    float3 p1 = patch[1].vPosition;
    float3 p2 = patch[2].vPosition;

    float3 pos = p0 * barycentricCoords.x +
                 p1 * barycentricCoords.y +
                 p2 * barycentricCoords.z;

    // îgÇÃèàóù
    pos.y += sin(pos.x * 0.5 + gMaterial.Time) * 0.3;

    DS_OUTPUT output;
    output.vPosition = mul(gMaterial.ViewProj, float4(pos, 1.0f));
    return output;
}
