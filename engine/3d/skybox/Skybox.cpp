#include "Skybox.h"

void Skybox::Initialize(DirectXCommon* dxCommon,const std::string& filePath)
{
	dxCommon_ = dxCommon;

	filePath_ = filePath;

	TextureManager::GetInstance()->LoadTexture(filePath_);

	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 24);

	vertexBufferView.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * 24);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// 右面
	vertexData_[0].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[1].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[3].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	// 左面
	vertexData_[4].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[5].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[7].position = { -1.0f, -1.0f,  1.0f, 1.0f };

	// 前面
	vertexData_[8].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[9].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[10].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertexData_[11].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	// 後面
	vertexData_[12].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[13].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[14].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[15].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	// 上面
	vertexData_[16].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[17].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[18].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[19].position = { 1.0f,  1.0f,  1.0f, 1.0f };

	// 下面
	vertexData_[20].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[21].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[22].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertexData_[23].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));

	// 書き込むためのアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	materialData_->shininess = 48.3f;

	materialData_->specularColor = { 1.0f,1.0f,1.0f };

	directionResource_ = dxCommon_->CreateBufferResource(sizeof(Direction));
	directionResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionData_));
	
	worldTransform_ = std::make_unique<WorldTransform>();

	worldTransform_->Initialize();

	worldTransform_->transform.scale = { 1000.0f, 1000.0f, 1000.0f};	

	worldTransform_->UpdateMatrix();

	directionResource_ = dxCommon_->CreateBufferResource(sizeof(Direction));
	directionResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionData_));

	InitializeIndexBuffer();

	CreateGraphicsPipeline();
}

void Skybox::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	directionData_->direction = viewProjection.transform.rotate;

	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばいい
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());  // PSOを設定

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);  // VBVを設定

	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(filePath_));

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight.GetDirectionalLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, worldTransform_->GetTransformResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLight.GetPointLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLight.GetSpotLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(7, directionResource_.Get()->GetGPUVirtualAddress());
	
	//dxCommon_->GetCommandList()->DrawInstanced(24, 1, 0, 0);

	dxCommon_->GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0); // インデックスバッファを使う場合はこっち
}

void Skybox::CreateRootSignature()
{
	HRESULT hr;

	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0からはじまる
	descriptorRange[0].NumDescriptors = 1; // 1から始まる
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  //PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで行う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数


	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[4].Descriptor.ShaderRegister = 1;

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[5].Descriptor.ShaderRegister = 2;

	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[6].Descriptor.ShaderRegister = 3;

	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[7].Descriptor.ShaderRegister = 4;

	descriptionRootSignature.pParameters = rootParameters;  // ルートパラメーター配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);  // 配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイナリフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;     // 多くのMipMapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリにする

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成
	rootSignature = nullptr;
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void Skybox::CreateGraphicsPipeline()
{
	HRESULT hr;
	
	// RootSignatureの作成
	CreateRootSignature();

	// InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//inputElementDescs[2].SemanticName = "NORMAL";
	//inputElementDescs[2].SemanticIndex = 0;
	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	// すべての要素数を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Skybox.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Skybox.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// 比較関数はLessEqual。近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;  // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };   // PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジ(形状)のタイプ.。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色をつけるか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	graphicsPipelineState = nullptr;
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void Skybox::InitializeIndexBuffer()
{
	// インデックスバッファの生成
	indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 36);

	indexBufferView_.BufferLocation = indexResource_.Get()->GetGPUVirtualAddress();

	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 36;

	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	// 右面
	indexData_[0] = 0;  indexData_[1] = 2;  indexData_[2] = 1;
	indexData_[3] = 1;  indexData_[4] = 2;  indexData_[5] = 3;

	// 左面
	indexData_[6] = 4;  indexData_[7] = 5;  indexData_[8] = 6;
	indexData_[9] = 5;  indexData_[10] = 7; indexData_[11] = 6;

	// 前面
	//indexData_[12] = 8;  indexData_[13] = 10; indexData_[14] = 9;
	//indexData_[15] = 9;  indexData_[16] = 10; indexData_[17] = 11;
	indexData_[12] = 8;  indexData_[13] = 9;  indexData_[14] = 10;
	indexData_[15] = 9;  indexData_[16] = 11; indexData_[17] = 10;

	// 後面
	//indexData_[18] = 12; indexData_[19] = 13; indexData_[20] = 14;
	//indexData_[21] = 13; indexData_[22] = 15; indexData_[23] = 14;

	indexData_[18] = 12; indexData_[19] = 14; indexData_[20] = 13;
	indexData_[21] = 13; indexData_[22] = 14; indexData_[23] = 15;

	// 上面
	//indexData_[24] = 16; indexData_[25] = 18; indexData_[26] = 17;
	//indexData_[27] = 17; indexData_[28] = 18; indexData_[29] = 19;
	indexData_[24] = 16; indexData_[25] = 17; indexData_[26] = 18;
	indexData_[27] = 17; indexData_[28] = 19; indexData_[29] = 18;

	// 下面
	//indexData_[30] = 20; indexData_[31] = 21; indexData_[32] = 22;
	//indexData_[33] = 21; indexData_[34] = 23; indexData_[35] = 22;
	indexData_[30] = 20; indexData_[31] = 22; indexData_[32] = 21;
	indexData_[33] = 21; indexData_[34] = 22; indexData_[35] = 23;
}

