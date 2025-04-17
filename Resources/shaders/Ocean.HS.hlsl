// 頂点シェーダーから渡されたデータ
struct VS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// Hull Shader が出力するコントロールポイント
struct HS_CONTROL_POINT_OUTPUT
{
    float3 vPosition : WORLDPOS;
};

// Patch単位で渡す定数（テッセレーション係数など）
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

// [1] Patch 定数計算
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
    InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
    uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    // ここで実際の距離などで可変にしても良い
    Output.EdgeTessFactor[0] = 15.0f;
    Output.EdgeTessFactor[1] = 15.0f;
    Output.EdgeTessFactor[2] = 15.0f;

    Output.InsideTessFactor = 15.0f;

    return Output;
}

// [2] Hull Shader 本体（1頂点ずつ呼ばれる）
[domain("tri")] // 三角パッチ
[partitioning("fractional_odd")] // 分割の方法（奇数・滑らか）
[outputtopology("triangle_cw")] // 出力トポロジ（時計回り三角形）
[outputcontrolpoints(NUM_CONTROL_POINTS)] // 出力頂点数
[patchconstantfunc("CalcHSPatchConstants")] // 定数関数を指定
HS_CONTROL_POINT_OUTPUT main(
    InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
    uint i : SV_OutputControlPointID,
    uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;

    // 単純にVSからの位置をそのまま渡す（変更も可）
    Output.vPosition = ip[i].vPosition;

    return Output;
}