#include "AABBLineObject.h"

namespace Kouro
{
    static constexpr uint32_t kVertexCount = 8;             //!< OBBの頂点数
    static constexpr uint32_t kEdgesPerFace = 3;            //!< 1面あたりの辺数
    static constexpr uint32_t kLineVertexCount = kVertexCount * kEdgesPerFace; //!< ライン描画用頂点数

    void AABBLineObject::Initialize(DirectXCommon* dxCommon)
    {
        ///	初期化処理

        // DirectXCommonのインスタンスをメンバ変数に記録
        dxCommon_ = dxCommon;

        // vertexResource_を生成
        CreateVertexResource();

        // vertexBufferView_を生成
        CreateVertexBufferView();
    }

    void AABBLineObject::Update()
    {
        /// 更新処理

        if (!aabbCollider_) {
            return; // AABBColliderが設定されていない場合は何もしない
        }

        const AABB& aabb = aabbCollider_->GetAABB();
        const Vector3& center = aabb.center;

        // min/max を center 分だけ相対位置に補正
        Vector3 min = aabb.min - center;
        Vector3 max = aabb.max - center;

        // AABBの8頂点（center原点とする）
        Vector3 vertices[8] = {
            { min.x, min.y, min.z }, // v0
            { max.x, min.y, min.z }, // v1
            { max.x, min.y, max.z }, // v2
            { min.x, min.y, max.z }, // v3
            { min.x, max.y, min.z }, // v4
            { max.x, max.y, min.z }, // v5
            { max.x, max.y, max.z }, // v6
            { min.x, max.y, max.z }  // v7
        };

        // AABBの辺を構成するインデックス（24個のライン）
        constexpr int lineIndices[kLineVertexCount] = {
            0, 1, 1, 2, 2, 3, 3, 0, // 下面
            4, 5, 5, 6, 6, 7, 7, 4, // 上面
            0, 4, 1, 5, 2, 6, 3, 7  // 側面
        };

        for (int i = 0; i < kLineVertexCount; ++i) {
            const Vector3& v = vertices[lineIndices[i]];
            vertexData_[i].position = { v.x, v.y, v.z, 1.0f };
            vertexData_[i].color = aabbCollider_->GetColor(); // AABBColliderの色を使用
        }
    }

    void AABBLineObject::Draw(const ViewProjection& viewProjection)
    {

    }

    void AABBLineObject::CreateVertexResource()
    {
        // 頂点データの確保
        vertexResource_ = dxCommon_->CreateBufferResource(sizeof(LineDrawer::VertexData) * kLineVertexCount);

        // 頂点データのマッピング
        vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    }

    void AABBLineObject::CreateVertexBufferView()
    {
        // VertexBufferViewの生成
        vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

        vertexBufferView_.SizeInBytes = UINT(sizeof(LineDrawer::VertexData) * kLineVertexCount);

        vertexBufferView_.StrideInBytes = sizeof(LineDrawer::VertexData);
    }
}