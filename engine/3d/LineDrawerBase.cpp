#include "LineDrawerBase.h"

void LineDrawerBase::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	pipeline_ = std::make_unique<Pipeline>();

	CreateRootSignature();

	CreatePipellineState();

	CreateLineResource();

	//CreateLineObject(Type::AABB);

	CreateLineObject(Type::Grid);

}

void LineDrawerBase::Update()
{
	for (std::list<std::unique_ptr<LineObject>>::iterator iterator = lineObjects_.begin(); iterator != lineObjects_.end();)
	{

		ImGui::DragFloat3("object.scale", &iterator->get()->transform.transform.scale.x, 0.01f);
		ImGui::DragFloat3("object.rotate", &iterator->get()->transform.transform.rotate.x, 0.01f);
		ImGui::DragFloat3("object.translate", &iterator->get()->transform.transform.translate.x, 0.01f);

		Matrix4x4 worldMatrix = MakeAffineMatrix(iterator->get()->transform.transform.scale, iterator->get()->transform.transform.rotate, iterator->get()->transform.transform.translate);

		iterator->get()->instancingData->matWorld = worldMatrix;

		iterator->get()->instancingData->color = { 0.0f,1.0f,0.0f,1.0f };

		++iterator;
	}
}

void LineDrawerBase::Draw(ViewProjection viewProjection)
{
	// プリミティブトポロジーを設定
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// ルートシグネチャを設定
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(pipeline_->rootSignature.Get());

	// PSOを設定
	dxCommon_->GetCommandList()->SetPipelineState(pipeline_->pipelineState.Get());
	
	for (std::list<std::unique_ptr<LineObject>>::iterator iterator = lineObjects_.begin(); iterator != lineObjects_.end();)
	{
		// VBVを設定
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &iterator->get()->vertexBufferView);

		dxCommon_->GetCommandList()->IASetIndexBuffer(&iterator->get()->indexBufferView);

		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvManager_->GetGPUDescriptorHandle(iterator->get()->srvIndex));

		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());

		dxCommon_->GetCommandList()->DrawIndexedInstanced(iterator->get()->vertexIndex, 1, 0, 0, 0);

		++iterator;
	}
}

void LineDrawerBase::CreateLineResource()
{
	CreateVertexResource();
}

#pragma region rootSignatureの生成
void LineDrawerBase::CreateRootSignature()
{
	HRESULT hr;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Tableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	/*----------------------------------------------------------
	* シリアライズしてバイナリにする
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	/*----------------------------------------------------------
	* バイナリを元RootSignatureを生成
	----------------------------------------------------------*/

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline_->rootSignature));
	assert(SUCCEEDED(hr));
}
#pragma endregion rootSignatureの生成

#pragma region pipelineの生成
void LineDrawerBase::CreatePipellineState()
{
	HRESULT hr;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "VERTEXINDEX";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32_SINT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/*----------------------------------------------------------
	* RasterizerStateの設定
	----------------------------------------------------------*/

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// 裏面(時計回り)を表示しない 
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;

	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	/*----------------------------------------------------------
	* Shaderのコンパイル
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerVS.hlsl", L"vs_6_0");

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerPS.hlsl", L"ps_6_0");

	/*----------------------------------------------------------
	* DepthStencilStateの設定
	----------------------------------------------------------*/

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	// 比較関数はLessEqual。近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	/*----------------------------------------------------------
	* BlendStateの設定
	----------------------------------------------------------*/

	D3D12_BLEND_DESC blendDesc{};

	// 全ての要素数を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	/*----------------------------------------------------------
	* GraphicsPipelineの設定
	----------------------------------------------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pipeline_->rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジー(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	
	// どのように画面に色をつけるか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipeline_->pipelineState));

	assert(SUCCEEDED(hr));
}
#pragma endregion pipelineの生成

# pragma region lineObjectの生成しlineObjects_にpush_back
void LineDrawerBase::CreateLineObject(Type type)
{
	// 新しいラインオブジェクトの生成と初期化
	std::unique_ptr<LineObject> newObject = std::make_unique<LineObject>();

	newObject->sphere.center = 0.0f;
	newObject->sphere.radius = 1.0f;

	///----------Vertex----------////

	// vertexResourceの生成
	newObject->vertexResource = CreateVertexResource();

	// マップ
	newObject->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->vertexData));
	
	// VertexBufferViewの生成
	CreateVertexBufferView(newObject.get());


	///----------Index----------////

	// indexResourceの生成
	newObject->indexResource = CreateIndexResource();

	// マップ
	newObject->indexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->indexData));

	// indexBufferViewの生成
	CreateIndexBufferView(newObject.get());

	///----------Instancing----------////

	// instancingResourceの生成
	newObject->instancingResource = CreateInstancingResource();

	// マップ
	newObject->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->instancingData));

	newObject->srvIndex = srvManager_->Allocate();

	srvManager_->CreateSRVforStructuredBuffer(newObject->srvIndex, newObject->instancingResource.Get(), kMaxLines, sizeof(LineForGPU));

	// vertexDataの生成

	if (type == Type::AABB)
	{
		WriteAABBVertexData(newObject.get(), { 0.5f,0.5f,0.5f });
	}
	else if (type == Type::Sphere)
	{
		WriteSphereVertexData(newObject.get());
	}
	else if (type == Type::Grid)
	{
		WriteGridVertexData(newObject.get());
	}
	newObject->transform.Initialize();

	newObject->transform.transform.scale = { 1.0f,1.0f,1.0f };

	lineObjects_.push_back(std::move(newObject));
}
#pragma endregion lineObjectの生成しlineObjects_にpush_back

#pragma region vertexResourceの生成
Microsoft::WRL::ComPtr<ID3D12Resource> LineDrawerBase::CreateVertexResource()
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = dxCommon_->CreateBufferResource(sizeof(VertexData) * kMaxLines);

	return resource;
}
#pragma endregion vertexResourceの生成

#pragma region vertexBufferViewの生成
void LineDrawerBase::CreateVertexBufferView(LineObject* object)
{
	// VertexBufferViewの生成
	object->vertexBufferView.BufferLocation = object->vertexResource->GetGPUVirtualAddress();

    object->vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * kMaxLines);

	object->vertexBufferView.StrideInBytes = sizeof(VertexData);
}
#pragma endregion vertexBufferViewの生成

#pragma region indexResourceの生成
Microsoft::WRL::ComPtr<ID3D12Resource> LineDrawerBase::CreateIndexResource()
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = dxCommon_->CreateBufferResource(sizeof(uint32_t) * kMaxLines * 2);

	return resource;
}
#pragma endregion indexResourceの生成

#pragma region indexBufferViewの生成
void LineDrawerBase::CreateIndexBufferView(LineObject* object)
{
	object->indexBufferView.BufferLocation = object->indexResource->GetGPUVirtualAddress();

	object->indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * kMaxLines * 2);

	object->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}
#pragma endregion indexBufferViewの生成

#pragma region instancingResourceの生成
Microsoft::WRL::ComPtr<ID3D12Resource> LineDrawerBase::CreateInstancingResource()
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = dxCommon_->CreateBufferResource(sizeof(LineForGPU) * kMaxLines);

	return resource;
}
#pragma endregion instancingResourceの生成

#pragma region sphere用のindexDataに書き込み
void LineDrawerBase::WriteSphereIndexData(LineObject* lineObject)
{
	int vertexIndex = 0;

	for (int i = 0; i < kMaxLines * 2;)
	{
		lineObject->indexData[i] = vertexIndex;
		++i;
		++vertexIndex;

		lineObject->indexData[i] = vertexIndex;
		++i;
	}
}
#pragma endregion sphere用でindexDataに書き込み

#pragma region AABB用でindexDataに書き込み
void LineDrawerBase::WriteAABBIndexData(LineObject* lineObject)
{
	for (int i = 0; i < lineObject->vertexIndex;)
	{
		lineObject->indexData[i] = i;
		++i;
	}
}
#pragma endregion AABB用でindexDataに書き込み

void LineDrawerBase::WriteGridIndexData(LineObject* lineObject)
{
	for (int i = 0; i < lineObject->vertexIndex;)
	{
		lineObject->indexData[i] = i;
		++i;
	}
}

#pragma region sphereの形でvertexDataを書き込む
void LineDrawerBase::WriteSphereVertexData(LineObject* lineObject)
{
	// 仮 VertexData書き込み

	int kSubdivision = 8;

	const float kLonEvery = 2.0f * float(std::numbers::pi) / kSubdivision;  // 経度分割1つ分の角度
	const float kLatEvery = 2.0f * float(std::numbers::pi) / kSubdivision;  // 緯度分割1つ分の角度

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			Vector3 a, b, c;

			a = { lineObject->sphere.radius * cosf(lon) * cosf(lat) + lineObject->sphere.center.x,
				  lineObject->sphere.radius * sinf(lon) + lineObject->sphere.center.y,
				  lineObject->sphere.radius * cosf(lon) * sinf(lat) + lineObject->sphere.center.z };

			b = { lineObject->sphere.radius * cosf(lon + kLonEvery) * cosf(lat) + lineObject->sphere.center.x,
				  lineObject->sphere.radius * sinf(lon + kLonEvery) + lineObject->sphere.center.y,
				  lineObject->sphere.radius * cosf(lon + kLonEvery) * sinf(lat) + lineObject->sphere.center.z };

			c = { lineObject->sphere.radius * cosf(lon) * cosf(lat + kLatEvery) + lineObject->sphere.center.x,
				  lineObject->sphere.radius * sinf(lon) + lineObject->sphere.center.y,
				  lineObject->sphere.radius * cosf(lon) * sinf(lat + kLatEvery) +lineObject->sphere.center.z };

			// 頂点データに線の始点と終点を追加
			lineObject->vertexData[lineObject->vertexIndex].position = { a.x,a.y,a.z,1.0f };
			++lineObject->vertexIndex;

			lineObject->vertexData[lineObject->vertexIndex].position = { b.x,b.y,b.z,1.0f };
			++lineObject->vertexIndex;

			lineObject->vertexData[lineObject->vertexIndex].position = { b.x,b.y,b.z,1.0f };
			++lineObject->vertexIndex;

			lineObject->vertexData[lineObject->vertexIndex].position = { c.x,c.y,c.z,1.0f };
			++lineObject->vertexIndex;
		}
	}

	WriteSphereIndexData(lineObject);
}
#pragma endregion sphereの形でvertexDataを書き込む

#pragma region AABBの形でvertexDataを書き込む
void LineDrawerBase::WriteAABBVertexData(LineObject* lineObject, Vector3 radius)
{
	AABB aabb;

	Vector3 center = { 0.0f,0.0f,0.0f };

	aabb.max = center + radius;
	aabb.min = center - radius;

	lineObject->vertexData[0].position = { aabb.min.x,aabb.min.y,aabb.min.z,1.0f };
	lineObject->vertexData[1].position = { aabb.min.x,aabb.min.y,aabb.max.z,1.0f };

	lineObject->vertexData[2].position = { aabb.min.x,aabb.min.y,aabb.max.z,1.0f };
	lineObject->vertexData[3].position = { aabb.max.x,aabb.min.y,aabb.max.z,1.0f };

	lineObject->vertexData[4].position = { aabb.max.x,aabb.min.y,aabb.max.z,1.0f };
	lineObject->vertexData[5].position = { aabb.max.x,aabb.min.y,aabb.min.z,1.0f };

	lineObject->vertexData[6].position = { aabb.max.x,aabb.min.y,aabb.min.z,1.0f };
	lineObject->vertexData[7].position = { aabb.min.x,aabb.min.y,aabb.min.z,1.0f };

	lineObject->vertexData[8].position = { aabb.min.x,aabb.min.y,aabb.min.z,1.0f };
	lineObject->vertexData[9].position = { aabb.min.x,aabb.max.y,aabb.min.z,1.0f };

	lineObject->vertexData[10].position = { aabb.min.x,aabb.max.y,aabb.min.z,1.0f };
	lineObject->vertexData[11].position = { aabb.min.x,aabb.max.y,aabb.max.z,1.0f };

	lineObject->vertexData[12].position = { aabb.min.x,aabb.max.y,aabb.max.z,1.0f };
	lineObject->vertexData[13].position = { aabb.min.x,aabb.min.y,aabb.max.z,1.0f };

	lineObject->vertexData[14].position = { aabb.max.x,aabb.min.y,aabb.min.z,1.0f };
	lineObject->vertexData[15].position = { aabb.max.x,aabb.max.y,aabb.min.z,1.0f };

	lineObject->vertexData[16].position = { aabb.max.x,aabb.max.y,aabb.max.z,1.0f };
	lineObject->vertexData[17].position = { aabb.min.x,aabb.max.y,aabb.max.z,1.0f };

	lineObject->vertexData[18].position = { aabb.max.x,aabb.max.y,aabb.max.z,1.0f };
	lineObject->vertexData[19].position = { aabb.max.x,aabb.min.y,aabb.max.z,1.0f };

	lineObject->vertexData[20].position = { aabb.max.x,aabb.max.y,aabb.max.z,1.0f };
	lineObject->vertexData[21].position = { aabb.max.x,aabb.max.y,aabb.min.z,1.0f };

	lineObject->vertexData[22].position = { aabb.max.x,aabb.max.y,aabb.min.z,1.0f }; 
	lineObject->vertexData[23].position = { aabb.min.x,aabb.max.y,aabb.min.z,1.0f };  

	lineObject->vertexData[24].position = { aabb.max.x,aabb.min.y,aabb.min.z,1.0f };
	lineObject->vertexData[25].position = { aabb.min.x,aabb.min.y,aabb.max.z,1.0f };


	lineObject->vertexIndex = 24;

	WriteAABBIndexData(lineObject);
}
#pragma endregion AABBの形でvertexDataを書き込む

void LineDrawerBase::WriteGridVertexData(LineObject* lineObject)
{
	float GridSize = 5.0f;
	int GridNum = 100;
	float GridSpacing = GridSize / GridNum;
	float halfGridSize = GridSize * GridNum * 0.5f;

	for (int i = 0; i <= GridNum; ++i) {
		float x = -halfGridSize + GridSize * i;
		float z = -halfGridSize + GridSize * i;

		lineObject->vertexData[lineObject->vertexIndex].position = { x, 0.0f, -halfGridSize, 1.0f };
		++lineObject->vertexIndex;
		lineObject->vertexData[lineObject->vertexIndex].position = { x, 0.0f, halfGridSize, 1.0f };
		++lineObject->vertexIndex;

		lineObject->vertexData[lineObject->vertexIndex].position = { -halfGridSize, 0.0f, z, 1.0f };
		++lineObject->vertexIndex;
		lineObject->vertexData[lineObject->vertexIndex].position = { halfGridSize, 0.0f, z, 1.0f };
		++lineObject->vertexIndex;
	}

	WriteGridIndexData(lineObject);
}