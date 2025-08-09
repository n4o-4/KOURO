#include "OBBLineObject.h"

static const uint32_t kMaxVertexNum = 24; // AABBの8頂点 * 3辺 = 24ライン

void OBBLineObject::Initialize(DirectXCommon* dxCommon)
{
	///	初期化処理

	// DirectXCommonのインスタンスをメンバ変数に記録
	dxCommon_ = dxCommon;

	// vertexResource_を生成
	CreateVertexResource();

	// vertexBufferView_を生成
	CreateVertexBufferView();
}

void OBBLineObject::Update()
{
    if (!obbCollider_) {
        return; // OBBが設定されていない場合は何もしない
    }

    const OBB& obb = obbCollider_->GetOBB();
    const Vector3& center = obb.center;

    // 回転軸
    const Vector3& xAxis = obb.axes[0]; // X軸方向ベクトル
    const Vector3& yAxis = obb.axes[1]; // Y軸方向ベクトル
    const Vector3& zAxis = obb.axes[2]; // Z軸方向ベクトル

    // ローカル空間での8頂点
    Vector3 localVertices[8] = {
        { +obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z }, // v0
        { +obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z }, // v1
        { +obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z }, // v2
        { +obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z }, // v3
        { -obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z }, // v4
        { -obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z }, // v5
        { -obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z }, // v6
        { -obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z }  // v7
    };

    // ライン描画用インデックス
    constexpr int lineIndices[24] = {
        0, 1, 1, 3, 3, 2, 2, 0, // 底面
        4, 5, 5, 7, 7, 6, 6, 4, // 上面
        0, 4, 1, 5, 2, 6, 3, 7  // 側面
    };

    // 頂点変換と格納
    for (int i = 0; i < 24; ++i) {
        const Vector3& local = localVertices[lineIndices[i]];

        // ローカル座標をワールド空間に変換（傾き適用）
        Vector3 world =
            xAxis * local.x +
            yAxis * local.y +
            zAxis * local.z +
            center;

        vertexData_[i].position = { world.x, world.y, world.z, 1.0f };
        vertexData_[i].color = obbCollider_->GetColor(); // OBBの色を使用
    }
}

void OBBLineObject::Draw(const ViewProjection& viewProjection)
{
}

void OBBLineObject::CreateVertexResource()
{
    // 頂点データの確保
    vertexResource_ = dxCommon_->CreateBufferResource(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

    // 頂点データのマッピング
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void OBBLineObject::CreateVertexBufferView()
{
    // VertexBufferViewの生成
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

    vertexBufferView_.SizeInBytes = UINT(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

    vertexBufferView_.StrideInBytes = sizeof(LineDrawer::VertexData);
}
