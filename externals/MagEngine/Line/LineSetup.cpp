#include "LineSetup.h"
#include "Logger.h"

void LineSetup::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) {
    // DirectXCommonの設定
    dxCommon_ = dxCommon;
    srvManager_ = srvManager;

    // ルートシグネチャの作成
    CreateRootSignature();

    // グラフィックスパイプラインの作成
    CreateGraphicsPipeline();

    // 頂点バッファビューの作成
    CreateVertexBufferView();

    // インデックスバッファビューの作成
    CreateIndexBufferView();
}

void LineSetup::CommonDrawSetup() {
    // コマンドリストの取得
    auto commandList = dxCommon_->GetCommandList();
    
    // ルートシグネイチャのセット
    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    
    // グラフィックスパイプラインステートをセット
    commandList->SetPipelineState(graphicsPipelineState_.Get());
    
    // プリミティブトポロジーをセットする(Line用にLINELISTに変更)
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineSetup::CreateRootSignature() {
    HRESULT hr;
    ID3D12Device* device = dxCommon_->GetDevice().Get();

    // ルートパラメータ
    D3D12_ROOT_PARAMETER rootParameters[1] = {};
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameters[0].Descriptor.ShaderRegister = 0;

    // ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.NumParameters = _countof(rootParameters);

    // シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
    hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) {
            Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            assert(false);
        }
    }
    assert(SUCCEEDED(hr));

    // ルートシグネチャの生成
    hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(hr));
}

void LineSetup::CreateGraphicsPipeline() {
    HRESULT hr;
    ID3D12Device* device = dxCommon_->GetDevice().Get();

    // 頂点レイアウト
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
    inputElementDescs[0].SemanticName = "POSITION";
    inputElementDescs[0].SemanticIndex = 0;
    inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs[1].SemanticName = "COLOR";
    inputElementDescs[1].SemanticIndex = 0;
    inputElementDescs[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    // シェーダーのコンパイル
    Microsoft::WRL::ComPtr<IDxcBlob> vsBlob;
    Microsoft::WRL::ComPtr<IDxcBlob> psBlob;

    // 頂点シェーダーのコンパイル
    vsBlob = dxCommon_->CompileShader(
        L"externals/MagEngine/Line/Line.VS.hlsl", 
        L"vs_6_0"
    );
    assert(vsBlob != nullptr);

    // ピクセルシェーダーのコンパイル
    psBlob = dxCommon_->CompileShader(
        L"externals/MagEngine/Line/Line.PS.hlsl", 
        L"ps_6_0"
    );
    assert(psBlob != nullptr);

    // BlendStateの設定
    D3D12_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // RasterizerStateの設定
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

    // パイプラインステートの設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
    graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
    graphicsPipelineStateDesc.InputLayout = {inputElementDescs, _countof(inputElementDescs)};
    graphicsPipelineStateDesc.VS = {vsBlob->GetBufferPointer(), vsBlob->GetBufferSize()};
    graphicsPipelineStateDesc.PS = {psBlob->GetBufferPointer(), psBlob->GetBufferSize()};
    graphicsPipelineStateDesc.BlendState = blendDesc;
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    // DepthStencilStateの設定
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // パイプラインステートオブジェクトの生成
    hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
    assert(SUCCEEDED(hr));
}

void LineSetup::CreateVertexBufferView() {
    HRESULT hr;
    ID3D12Device* device = dxCommon_->GetDevice().Get();

    // 頂点データ (ダミーデータ)
    LineVertex vertices[2] = {
        {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
    };

    // 頂点バッファリソース作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeof(vertices);
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    // 頂点バッファの生成
    hr = device->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_)
    );
    assert(SUCCEEDED(hr));

    // 頂点バッファへのデータ転送
    void* vertexData;
    hr = vertexResource_->Map(0, nullptr, &vertexData);
    assert(SUCCEEDED(hr));
    memcpy(vertexData, vertices, sizeof(vertices));
    vertexResource_->Unmap(0, nullptr);

    // 頂点バッファビューの作成
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(vertices);
    vertexBufferView_.StrideInBytes = sizeof(LineVertex);
}

void LineSetup::CreateIndexBufferView() {
    HRESULT hr;
    ID3D12Device* device = dxCommon_->GetDevice().Get();

    // インデックスデータ (ダミーデータ)
    uint16_t indices[2] = {0, 1};

    // インデックスバッファリソース作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeof(indices);
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    // インデックスバッファの生成
    hr = device->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexResource_)
    );
    assert(SUCCEEDED(hr));

    // インデックスバッファへのデータ転送
    void* indexData;
    hr = indexResource_->Map(0, nullptr, &indexData);
    assert(SUCCEEDED(hr));
    memcpy(indexData, indices, sizeof(indices));
    indexResource_->Unmap(0, nullptr);

    // インデックスバッファビューの作成
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(indices);
    indexBufferView_.Format = DXGI_FORMAT_R16_UINT;
}