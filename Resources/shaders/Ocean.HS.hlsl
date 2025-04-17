// ���_�V�F�[�_�[����n���ꂽ�f�[�^
struct VS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// Hull Shader ���o�͂���R���g���[���|�C���g
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// Patch�P�ʂœn���萔�i�e�b�Z���[�V�����W���Ȃǁj
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

// [1] Patch �萔�v�Z
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
    InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
    uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    // �����Ŏ��ۂ̋����Ȃǂŉςɂ��Ă��ǂ�
    Output.EdgeTessFactor[0] = 15.0f;
    Output.EdgeTessFactor[1] = 15.0f;
    Output.EdgeTessFactor[2] = 15.0f;

    Output.InsideTessFactor = 15.0f;

    return Output;
}

// [2] Hull Shader �{�́i1���_���Ă΂��j
[domain("tri")] // �O�p�p�b�`
[partitioning("fractional_odd")] // �����̕��@�i��E���炩�j
[outputtopology("triangle_cw")] // �o�̓g�|���W�i���v���O�p�`�j
[outputcontrolpoints(NUM_CONTROL_POINTS)] // �o�͒��_��
[patchconstantfunc("CalcHSPatchConstants")] // �萔�֐����w��
HS_CONTROL_POINT_OUTPUT main(
    InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
    uint i : SV_OutputControlPointID,
    uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;

    // �P����VS����̈ʒu�����̂܂ܓn���i�ύX���j
    Output.vPosition = ip[i].vPosition;

    return Output;
}