#include "Line.h"
#include "LineSetup.h"
#include "DirectXCommon.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include <numbers>

void Line::Initialize(LineSetup* lineSetup) {
    // ラインセットアップの取得
    lineSetup_ = lineSetup;
    
    // 頂点バッファの作成
    CreateVertexBuffer();
    
    // トランスフォーメーションマトリックスバッファの作成
    CreateTransformationMatrixBuffer();
    
    // ワールド行列の初期化
    transform_ = { {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };
    
    // カメラの取得
    camera_ = lineSetup_->GetDefaultCamera();
}

void Line::Update() {
    // カメラの取得
    camera_ = lineSetup_->GetDefaultCamera();

    // ワールド行列の作成
    Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    Matrix4x4 worldViewProjectionMatrix;

    if (camera_) {
        // ビュー行列とプロジェクション行列を取得
        const Matrix4x4& viewMatrix = camera_->matView_;
        const Matrix4x4& projectionMatrix = camera_->matProjection_;

        // 行列の乗算（ワールド → ビュー → プロジェクション）
        Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);
        worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix);
    } else {
        worldViewProjectionMatrix = worldMatrix;
    }

    // 定数バッファへの書き込み
    transformationMatrixData_->WVP = worldViewProjectionMatrix;
    transformationMatrixData_->World = worldMatrix;
    transformationMatrixData_->WorldInvTranspose = Inverse(worldMatrix);
}

void Line::DrawLine(const Vector3& start, const Vector3& end, const Vector4& color) {
    // 頂点データを追加
    LineVertex startVertex = { start, color };
    LineVertex endVertex = { end, color };
    vertices_.push_back(startVertex);
    vertices_.push_back(endVertex);
}

void Line::Draw() {
    // 描画するラインがない場合は何もしない
    if (vertices_.empty()) return;
    
    // 描画設定
    void* pData;
    // バーテックスバッファのマップ
    vertexBuffer_->Map(0, nullptr, &pData);
    // メモリコピー
    memcpy(pData, vertices_.data(), sizeof(LineVertex) * vertices_.size());
    // バーテックスバッファのアンマップ
    vertexBuffer_->Unmap(0, nullptr);

    // 描画設定
    auto commandList = lineSetup_->GetDXCommon()->GetCommandList();
    
    // バーテックスバッファビューをセット
    commandList->IASetVertexBuffers(0, 1, &lineSetup_->GetVBV());
    commandList->IASetIndexBuffer(&lineSetup_->GetIBV());
    
    // 変換行列バッファをセット
    commandList->SetGraphicsRootConstantBufferView(0, transformationMatrixBuffer_->GetGPUVirtualAddress());
    
    // 描画
    commandList->DrawInstanced(static_cast<UINT>(vertices_.size()), 1, 0, 0);
}

void Line::ClearLines() {
    // 頂点データのクリア
    vertices_.clear();
}

void Line::CreateVertexBuffer() {
    auto dxCommon = lineSetup_->GetDXCommon();
    auto device = dxCommon->GetDevice();

    // 頂点バッファの作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeof(LineVertex) * 2048; // 最大頂点数
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 頂点バッファの生成
    HRESULT hr = device->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer_));
    assert(SUCCEEDED(hr));
}

void Line::CreateTransformationMatrixBuffer() {
    auto dxCommon = lineSetup_->GetDXCommon();
    auto device = dxCommon->GetDevice();

    // 変換行列バッファの作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = sizeof(TransformationMatrix);
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // 変換行列バッファの生成
    HRESULT hr = device->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&transformationMatrixBuffer_));
    assert(SUCCEEDED(hr));

    // マップ
    transformationMatrixBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
}