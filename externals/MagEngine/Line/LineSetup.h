#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "ViewProjection.h"

class LineSetup {
public:
    // 初期化
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

    // 共通描画設定
    void CommonDrawSetup();

private:
    // ルートシグネチャの作成
    void CreateRootSignature();

    // グラフィックスパイプラインの作成
    void CreateGraphicsPipeline();

    // 頂点バッファビューの作成
    void CreateVertexBufferView();

    // インデックスバッファビューの作成
    void CreateIndexBufferView();

public:
    // DirectXCommonの取得
    DirectXCommon* GetDXCommon() const { return dxCommon_; }

    // SrvManagerの取得
    SrvManager* GetSrvManager() const { return srvManager_; }

    // デフォルトカメラの設定
    void SetDefaultCamera(ViewProjection* camera) { this->defaultCamera_ = camera; }
    
    // デフォルトカメラの取得
    ViewProjection* GetDefaultCamera() { return defaultCamera_; }

    // 頂点バッファビューの取得
    D3D12_VERTEX_BUFFER_VIEW GetVBV() const { return vertexBufferView_; }

    // インデックスバッファビューの取得
    D3D12_INDEX_BUFFER_VIEW GetIBV() const { return indexBufferView_; }

private:
    // DirectXCommonポインタ
    DirectXCommon* dxCommon_ = nullptr;
    
    // SrvManagerポインタ
    SrvManager* srvManager_ = nullptr;

    // デフォルトカメラ
    ViewProjection* defaultCamera_ = nullptr;

    // ルートシグネチャ
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
    
    // パイプラインステート
    Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

    // インデックスバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;

    // 頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    
    // インデックスバッファビュー
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
};