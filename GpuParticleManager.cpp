#include "GpuParticleManager.h"
#include "TextureManager.h"
#include "ModelLoader.h"

GpuParticleManager::GpuParticleManager(GpuContext context)
{
	device_ = context.d3d12Context.device;

	commandList_ = context.d3d12Context.commandList;

	srvManager_ = context.srvManager;

	uavManager_ = context.uavManager;
}

void GpuParticleManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	CreateResources();

	CreatePipelineSets();

	ModelLoader loader;

	CreateParticleGroup("normal", "Resources/circle.png", loader.LoadModel("plane.obj"));
};

void GpuParticleManager::Update()
{
	srvManager_->PreDraw();
	uavManager_->PreDraw();

	for (auto it = particleGroups_.begin(); it != particleGroups_.end(); ++it) {
		auto& group = it->second;
		
		// noiseUpdate
		dxCommon_->GetCommandList()->SetComputeRootSignature(pipelineSets_.find("noiseUpdate")->second->rootSignature.Get());
		dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_.find("noiseUpdate")->second->pipelineState.Get());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, group.particleResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, group.counterResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, group.freeListResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(3, group.noiseUpdateListResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(4, perFrameResource_.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->Dispatch(512, 1, 1);

		// baseUpdate
		dxCommon_->GetCommandList()->SetComputeRootSignature(pipelineSets_.find("baseUpdate")->second->rootSignature.Get());
		dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_.find("baseUpdate")->second->pipelineState.Get());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, group.particleResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, group.counterResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, group.freeListResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(3, group.baseUpdateListResource.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(4, perFrameResource_.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->Dispatch(512, 1, 1);
	}
}

void GpuParticleManager::Draw(ViewProjection viewPro)
{
	Matrix4x4 view = viewPro.matView_;

	Matrix4x4 projection = viewPro.matProjection_;

	perView_->viewProjection = Multiply(view, projection);

	Matrix4x4 backToFrontMatrix = MakeRotateZMatrix(std::numbers::pi_v<float>);

	perView_->billboardMatrix = Multiply(backToFrontMatrix, viewPro.matWorld_);

	for (auto it = particleGroups_.begin(); it != particleGroups_.end(); ++it)
	{
		auto& group = it->second;

		// rootSignature
		dxCommon_->GetCommandList()->SetGraphicsRootSignature(pipelineSets_.find("draw")->second->rootSignature.Get());

		// pipeLineState
		dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_.find("draw")->second->pipelineState.Get());

		// 
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &group.vertexBufferView);

		// 
		dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, group.materialResource.Get()->GetGPUVirtualAddress());

		// SRV
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(group.particleSrvIndex));

		// 
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(group.textureIndex));

		// 
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, perViewResource_.Get()->GetGPUVirtualAddress());

		// 
		dxCommon_->GetCommandList()->DrawInstanced(6, kMaxParticleCount, 0, 0);
	}
}

void GpuParticleManager::ParticleInitialize(ParticleGroup group)
{
	srvManager_->PreDraw();

	// RootSignatureの設定（initialize用）
	dxCommon_->GetCommandList()->SetComputeRootSignature(pipelineSets_.find("initialize")->second.get()->rootSignature.Get());

	// PipelineStateの設定（initialize用）
	dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_["initialize"]->pipelineState.Get());

	// UAVの設定（u0: パーティクル, u1: カウンタ, u2: フリーリスト
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, group.particleResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, group.counterResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, group.freeListResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(3, group.baseUpdateListResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(4, group.noiseUpdateListResource.Get()->GetGPUVirtualAddress());

	// ComputeShaderの実行
	dxCommon_->GetCommandList()->Dispatch(kMaxParticleCount / kCSMaxParticleCount, 1, 1);
}

void GpuParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath, std::vector<VertexData> vertices)
{
	// キーを検索 なかった場合新しくグループを作る
	if (particleGroups_.find(name) == particleGroups_.end())
	{
		// 新しいパーティクルグループ
		ParticleGroup newGroup;

		newGroup.particleResource = dxCommon_->CreateComputeBufferResource(sizeof(Particle) * kMaxParticleCount);

		newGroup.counterResource = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t));

		newGroup.freeListResource = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t) * kMaxParticleCount);

		newGroup.baseUpdateListResource = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t) * kMaxParticleCount);

		newGroup.noiseUpdateListResource = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t) * kMaxParticleCount);

		// SRV
		newGroup.particleSrvIndex = srvManager_->Allocate();
		srvManager_->CreateSRVforStructuredBuffer(newGroup.particleSrvIndex, newGroup.particleResource.Get(), kMaxParticleCount, sizeof(Particle));

		// UAV
		newGroup.particleUavIndex = uavManager_->Allocate();
		uavManager_->CreateUAVforStructuredBuffer(newGroup.particleUavIndex, newGroup.particleResource.Get(), kMaxParticleCount, sizeof(Particle));

		// vertex

		newGroup.vertexResource = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertices.size());

		newGroup.vertexBufferView.BufferLocation = newGroup.vertexResource.Get()->GetGPUVirtualAddress();

		newGroup.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());

		newGroup.vertexBufferView.StrideInBytes = sizeof(VertexData);

		VertexData* vertexData;
		newGroup.vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		std::memcpy(vertexData, vertices.data(), sizeof(VertexData) * vertices.size());

		// material
		newGroup.materialResource = dxCommon_->CreateBufferResource(sizeof(Material));
		newGroup.material = nullptr;
		newGroup.materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&newGroup.material));

		newGroup.material->color = { 1.0f,1.0f,1.0f,1.0f };
		newGroup.material->uvTransform = MakeIdentity4x4();

		newGroup.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

		ParticleInitialize(newGroup);

		particleGroups_[name] = newGroup;
	}
}

void GpuParticleManager::LineEmit(std::string groupName, uint32_t lineSrvIndex, uint32_t lineCount, Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource, Matrix4x4 world)
{
	transform_->matWorld = world;

	// 1. Compute RootSignature ��Z�b�g
	dxCommon_->GetCommandList()->SetComputeRootSignature(pipelineSets_.find("modelEdgeEmitter")->second.get()->rootSignature.Get());

	// 2. Pipeline State ��Z�b�g
	dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_.find("modelEdgeEmitter")->second.get()->pipelineState.Get());

	if (particleGroups_.find(groupName) == particleGroups_.end()) return;

	ParticleGroup group = particleGroups_.find(groupName)->second;

	// 3. Compute Root �� Descriptor / CBV / UAV 
	dxCommon_->GetCommandList()->SetComputeRootDescriptorTable(0, srvManager_->GetGPUDescriptorHandle(lineSrvIndex));
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(1, transformResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, group.particleResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(3, group.counterResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(4, group.freeListResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(5, emitterResource.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(6, perFrameResource_.Get()->GetGPUVirtualAddress());


	// Dispatch�̎��s
	dxCommon_->GetCommandList()->Dispatch(static_cast<UINT>(lineCount), 1, 1);

	D3D12_RESOURCE_BARRIER barrier1{};
	barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier1.UAV.pResource = group.particleResource.Get();
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);
}

void GpuParticleManager::CreateResources()
{
	perViewResource_ = dxCommon_->CreateBufferResource(sizeof(PerView));
	perView_ = nullptr;
	perViewResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&perView_));

	emitterResource_ = dxCommon_->CreateBufferResource(sizeof(EmitterSphere));
	emitter_ = nullptr;
	emitterResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));
	emitter_->emit = 0;
	emitter_->frequency = 0.0f;
	emitter_->frequencyTime = 0.0f;
	emitter_->count = 512;

	perFrameResource_ = dxCommon_->CreateBufferResource(sizeof(PerFrame));
	perFrame_ = nullptr;
	perFrameResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	transformResource_ = dxCommon_->CreateBufferResource(sizeof(Transform));
	transform_ = nullptr;
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transform_));
}

void GpuParticleManager::CreatePipelineSets()
{
	CreateInitializePipelineSet();

	CreateBaseUpdatePipelineSet();

	CreateNoiseUpdatePipelineSet();

	CreateGraphicsPipelineSet();
}

void GpuParticleManager::CreateInitializePipelineSet()
{
	// pipelineSetの生成（Compute用のパイプラインセットを確保）
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignatureの記述構造体を初期化
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// Input Assembler（頂点入力レイアウト）を許可するフラグを設定
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter配列（UAVを5つ使用する）
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAVレジスタ0番を設定（出力用バッファなどに使用）
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// UAVレジスタ1番を設定（別の出力リソースなどに使用）
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	// UAVレジスタ2番を設定（さらに別の出力リソースなどに使用）
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	// UAVレジスタ3番を設定（別の出力リソースなどに使用）
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 3;

	// UAVレジスタ3番を設定（さらに別の出力リソースなどに使用）
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 4;

	// ルートパラメータをRootSignatureに登録
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// RootSignatureをシリアライズ（バイナリ化）
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	// エラーが発生した場合はログを出力
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// シリアライズ結果から実際のRootSignatureを作成
	hr = dxCommon_->GetDevice()->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pSet->rootSignature)
	);

	// ComputePipelineStateを作成（初期化用のComputeShaderを指定）
	CreateComputePipelineState(pSet.get(), "Resources/shaders/InitializeParticle.CS.hlsl");

	// unordered_map に追加
	pipelineSets_["initialize"] = std::move(pSet);
}

void GpuParticleManager::CreateBaseUpdatePipelineSet()
{
	// 新しい Compute 用の PipelineSet を生成
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignature の記述構造体を初期化
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// Input Assembler（頂点入力レイアウト）の使用を許可するフラグを設定
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter 配列を初期化（UAV×4 + CBV×1）
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAV レジスタ 0 番を設定（ComputeShader の出力先バッファなどに使用）
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// UAV レジスタ 1 番を設定（別の出力リソースなどに使用）
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	// UAV レジスタ 2 番を設定（さらに別の出力リソースなどに使用）
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	// UAV レジスタ 2 番を設定（さらに別の出力リソースなどに使用）
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 3;

	// CBV レジスタ 0 番を設定（定数バッファ：パーティクル全体の共通データなどに使用）
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 0;

	// ルートパラメータ配列を RootSignature 記述に登録
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// RootSignature をシリアライズ（バイナリデータに変換）
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signatureBlob,
		&errorBlob
	);

	// もしシリアライズに失敗した場合、エラーログを出力
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// シリアライズされたデータから RootSignature を生成
	hr = dxCommon_->GetDevice()->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pSet->rootSignature)
	);

	// ComputePipelineState を作成（更新用 ComputeShader を指定）
	CreateComputePipelineState(pSet.get(), "Resources/shaders/BaseUpdateParticle.CS.hlsl");

	// unordered_map に追加
	pipelineSets_["baseUpdate"] = std::move(pSet);
}

void GpuParticleManager::CreateNoiseUpdatePipelineSet()
{
	// 新しい Compute 用の PipelineSet を生成
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignature の記述構造体を初期化
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// Input Assembler（頂点入力レイアウト）の使用を許可するフラグを設定
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter 配列を初期化（UAV×4 + CBV×1）
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAV レジスタ 0 番を設定（ComputeShader の出力先バッファなどに使用）
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// UAV レジスタ 1 番を設定（別の出力リソースなどに使用）
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	// UAV レジスタ 2 番を設定（さらに別の出力リソースなどに使用）
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	// UAV レジスタ 2 番を設定（さらに別の出力リソースなどに使用）
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 3;

	// CBV レジスタ 0 番を設定（定数バッファ：パーティクル全体の共通データなどに使用）
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 0;

	// ルートパラメータ配列を RootSignature 記述に登録
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// RootSignature をシリアライズ（バイナリデータに変換）
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signatureBlob,
		&errorBlob
	);

	// もしシリアライズに失敗した場合、エラーログを出力
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// シリアライズされたデータから RootSignature を生成
	hr = dxCommon_->GetDevice()->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pSet->rootSignature)
	);

	// ComputePipelineState を作成（更新用 ComputeShader を指定）
	CreateComputePipelineState(pSet.get(), "Resources/shaders/NoiseUpdateParticle,CS.hlsl");

	// unordered_map に追加
	pipelineSets_["noiseUpdate"] = std::move(pSet);
}

void GpuParticleManager::CreateModelEdgeEmitterPipelineSet()
{
	// 新しい Compute 用の PipelineSet を生成
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignature の設定
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// SRV（インスタンシング用）のディスクリプタレンジ設定
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;                         // t0
	descriptorRangeForInstancing[0].NumDescriptors = 1;                             // 1つだけ
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;    // SRVタイプ
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParameters[7] = {};

	// SRV（インスタンシングデータ）
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	// CBV (b0)
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	// UAV (u0)
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 0;

	// UAV (u1)
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	// UAV (u2)
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 2;

	// CBV (b1)
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[5].Descriptor.ShaderRegister = 1;

	// CBV (b2)
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[6].Descriptor.ShaderRegister = 2;

	// ルートシグネチャにパラメータを登録
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// RootSignature をシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	// エラー時はログ出力
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// RootSignature の生成
	hr = device_->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pSet->rootSignature)
	);

	// ComputePipelineState の作成
	CreateComputePipelineState(pSet.get(), "Resources/shaders/EmitParticleLine.CS.hlsl");

	// unordered_map に追加
	pipelineSets_["modelEdgeEmitter"] = std::move(pSet);
}

void GpuParticleManager::CreateComputePipelineState(PipelineSet* pipelineSet, std::string csFileName)
{
	// Shaderファイル名（UTF-8）をwstring（UTF-16）に変換
	std::wstring wShaderPath(csFileName.begin(), csFileName.end());

	// ComputeShaderをコンパイル（cs_6_0プロファイルでコンパイル）
	Microsoft::WRL::ComPtr<IDxcBlob> computeShaderBlob = dxCommon_->CompileShader(wShaderPath.c_str(), L"cs_6_0");

	// Computeパイプラインステートの記述構造体を初期化
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	// コンパイル済みのシェーダーバイナリを登録
	computePipelineStateDesc.CS = {
		.pShaderBytecode = computeShaderBlob->GetBufferPointer(),
		.BytecodeLength = computeShaderBlob->GetBufferSize()
	};

	// 使用するRootSignatureを設定
	computePipelineStateDesc.pRootSignature = pipelineSet->rootSignature.Get();

	// ComputePipelineStateを作成
	hr = dxCommon_->GetDevice()->CreateComputePipelineState(
		&computePipelineStateDesc,
		IID_PPV_ARGS(&pipelineSet->pipelineState)
	);
}
void GpuParticleManager::CreateGraphicsPipelineSet()
{
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// rootSignature�̐���
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// rootParameter�̐���
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// material
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// instancing
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	// 
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	// 
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// sampler�̐���
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // �o�C�i���t�B���^
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1�͈̔͊O����s�[�g
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // X���W
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // Y���W
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // ��r���Ȃ�
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ������MipMap��g��
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// �V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// �o�C�i�������RootSignature�𐶐�
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pSet->rootSignature));
	assert(SUCCEEDED(hr));

	// InputLayout�̐ݒ�
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// RasterizerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// ����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	// �O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shader�̃R���p�C��
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Particle.CS.VS.hlsl", L"vs_6_0");

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Particle.CS.PS.hlsl", L"ps_6_0");

	// DepthStencilState�̐ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depth�̋@�\��L��������
	depthStencilDesc.DepthEnable = true;

	// �������݂��܂�
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// ��r�֐���LessEqual�B�߂���Ε`�悳���
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	// BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};

	// �S�Ă̗v�f�����������
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	blendDesc.RenderTarget[0].BlendEnable = TRUE;


	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;

	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pSet->rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	// ��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// ���p����g�|���W�[(�`��)�̃^�C�v�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �ǂ̂悤�ɉ�ʂɐF����邩
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencil�̐ݒ�
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ���ۂɐ���
	pSet->pipelineState = nullptr;
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pSet->pipelineState));
	assert(SUCCEEDED(hr));

	// unordered_map に追加
	pipelineSets_["draw"] = std::move(pSet);
}