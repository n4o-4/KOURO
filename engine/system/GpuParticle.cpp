#include "GpuParticle.h"
#include "TextureManager.h"
#include "ModelLoader.h"

std::unique_ptr<GpuParticle> GpuParticle::instance = nullptr;

GpuParticle* GpuParticle::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<GpuParticle>();
	}

	return instance.get();
}

void GpuParticle::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, UavManager* uavManagedr)
{
	// �����o�ϐ��ɋL�^
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	uavManager_ = uavManagedr;

	// ���\�[�X�̐���
	CreateResource();

	// ���_���\�[�X�̐���
	CreateVertexResource();

	// ���_�o�b�t�@�r���[�̐���
	CreateVertexBufferView();

	// �p�[�r���\���\�[�X�̐���
	CreatePerViewResource();

	// �}�e���A�����\�[�X�̐���
	CreateMaterialResource();

	// �G�~�b�^�[���\�[�X�̐���
	CreateEmitterResource();

	// 
	CreatePerFrameResource();

	CreateTransformResource();

	CreateLineCountResource();

	// SRV�̍쐬
	srvIndex_ = srvManager_->Allocate();
	srvManager_->CreateSRVforStructuredBuffer(srvIndex_, particleResource_.Get(), kMaxParticleCount, sizeof(ParticleCS));

	// UAV�̍쐬
	uavIndex_ = uavManager_->Allocate();
	uavManager_->CreateUAVforStructuredBuffer(uavIndex_, particleResource_.Get(), kMaxParticleCount, sizeof(ParticleCS));

	// �p�C�v���C���Z�b�g�̍쐬
	CreatePipelineSet();

	materialData_.textureFilePath = "Resources/circle.png";
	TextureManager::GetInstance()->LoadTexture(materialData_.textureFilePath);
	materialData_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(materialData_.textureFilePath);

	material_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->uvTransform = MakeIdentity4x4();

	CreateLineSegment("enemy/enemy.obj");

	// �q�[�v�̐ݒ�
	srvManager_->PreDraw();

	// rootSignature�̐ݒ�
	dxCommon_->GetCommandList()->SetComputeRootSignature(initializePipelineSet_->rootSignature.Get());

	// pipeLineState�̐ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(initializePipelineSet_->pipelineState.Get());

	// UAV�̐ݒ�
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, particleResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, counterResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, freeListResource_.Get()->GetGPUVirtualAddress());

	// Dispatch�̎��s
	dxCommon_->GetCommandList()->Dispatch(1, 1, 1);
}

void GpuParticle::Finalize()
{
	instance.reset();
}

void GpuParticle::Update(ViewProjection viewProjection)
{
	Matrix4x4 view = viewProjection.GetViewMatrix();

	Matrix4x4 projection = viewProjection.GetProjectionMatrix();

	perView_->viewProjection = Multiply(view, projection);

	Matrix4x4 backToFrontMatrix = MakeRotateZMatrix(std::numbers::pi_v<float>);

	perView_->billboardMatrix = Multiply(backToFrontMatrix, viewProjection.GetWorldMatrix());

	//emitter_->frequencyTime += kDeltaTime;

	if (emitter_->frequency <= emitter_->frequencyTime)
	{
		emitter_->frequencyTime -= emitter_->frequency;
		emitter_->emit = 1;
	}
	else
	{
		emitter_->emit = 0;
	}

	// �q�[�v�̐ݒ�
	srvManager_->PreDraw();

	// rootSignature�̐ݒ�
	dxCommon_->GetCommandList()->SetComputeRootSignature(emitPipelineSet_->rootSignature.Get());

	// pipeLineState�̐ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(emitPipelineSet_->pipelineState.Get());

	// UAV�̐ݒ�
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, particleResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, counterResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, freeListResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(3, emitterResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(4, perFrameResource_.Get()->GetGPUVirtualAddress());

	// Dispatch�̎��s
	//dxCommon_->GetCommandList()->Dispatch(1, 1, 1);

	D3D12_RESOURCE_BARRIER barrier1{};
	barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier1.UAV.pResource = particleResource_.Get();
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

	D3D12_RESOURCE_BARRIER barrier2{};
	barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier2.UAV.pResource = counterResource_.Get();
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

	D3D12_RESOURCE_BARRIER barrier3{};
	barrier3.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier3.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier3.UAV.pResource = freeListResource_.Get();
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier3);

	// rootSignature�̐ݒ�
	dxCommon_->GetCommandList()->SetComputeRootSignature(updatePipelineSet_->rootSignature.Get());

	// pipeLineState�̐ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(updatePipelineSet_->pipelineState.Get());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, particleResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, counterResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, freeListResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(3, perFrameResource_.Get()->GetGPUVirtualAddress());

	// Dispatch�̎��s
	dxCommon_->GetCommandList()->Dispatch(512, 1, 1);
}

void GpuParticle::Draw()
{
	// rootSignature�̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(graphicsPipelineSet_->rootSignature.Get());
	// pipeLineState�̐ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(graphicsPipelineSet_->pipelineState.Get());

	// ���_�o�b�t�@�̐ݒ�
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// �v���~�e�B�u�g�|���W�[�̐ݒ�
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �萔�o�b�t�@�̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	// SRV�̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(srvIndex_));

	// �e�N�X�`���̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(materialData_.textureIndex));

	// �p�[�r���\���\�[�X�̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, perViewResource_.Get()->GetGPUVirtualAddress());

	// �`��R�}���h
	dxCommon_->GetCommandList()->DrawInstanced(6, kMaxParticleCount, 0, 0);
}

void GpuParticle::CreateLineSegment(std::string filePath)
{
	ModelLoader loader;

	std::vector<Line::Vertex> vertices = loader.LoadLineModel(filePath);

	// ���_������ł���O��iLINELIST�j
	// 2���Ƃɐ�����쐬
	for (size_t i = 0; i + 1 < vertices.size(); i += 2)
	{
		Particle::LineSegment seg;
		seg.start = { vertices[i].position.x, vertices[i].position.y, vertices[i].position.z };
		seg.end = { vertices[i + 1].position.x, vertices[i + 1].position.y, vertices[i + 1].position.z };
		seg.pad1 = 0.0f;
		seg.pad2 = 0.0f;

		lineSegments_.push_back(seg);
	}

	// lineSegment���\�[�X�̐���
	CreateLineSegmentResource();
}

void GpuParticle::LineEmit(std::string groupName, Matrix4x4 world)
{
	transform_->matWorld = world;

	// 1. Compute RootSignature ��Z�b�g
	dxCommon_->GetCommandList()->SetComputeRootSignature(lineEmitPipelineSet_->rootSignature.Get());

	// 2. Pipeline State ��Z�b�g
	dxCommon_->GetCommandList()->SetPipelineState(lineEmitPipelineSet_->pipelineState.Get());

	// 3. Compute Root �� Descriptor / CBV / UAV ��Z�b�g
	dxCommon_->GetCommandList()->SetComputeRootDescriptorTable(0, srvManager_->GetGPUDescriptorHandle(lineSegmentSrvIndex_));
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(1, transformResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, particleResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(3, counterResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(4, freeListResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(5, emitterResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(6, perFrameResource_.Get()->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetComputeRootConstantBufferView(7, lineCountResource_.Get()->GetGPUVirtualAddress());


	// Dispatch�̎��s
	dxCommon_->GetCommandList()->Dispatch(static_cast<UINT>(lineSegments_.size()), 1, 1);

	D3D12_RESOURCE_BARRIER barrier1{};
	barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier1.UAV.pResource = particleResource_.Get();
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);
}

void GpuParticle::CreateResource()
{
	particleResource_ = dxCommon_->CreateComputeBufferResource(sizeof(ParticleCS) * kMaxParticleCount);

	counterResource_ = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t));

	freeListResource_ = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t) * kMaxParticleCount);
}

void GpuParticle::CreateVertexResource()
{
	modelData_.vertices.push_back({ { -0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }); // ����
	modelData_.vertices.push_back({ {  0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }); // �E��
	modelData_.vertices.push_back({ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }); // ����

	modelData_.vertices.push_back({ {  0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }); // �E��
	modelData_.vertices.push_back({ {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }); // �E��
	modelData_.vertices.push_back({ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }); // ����

	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
}

void GpuParticle::CreateVertexBufferView()
{
	// ���\�[�X�̐擪�A�h���X
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();

	// �g�p���郊�\�[�X�̃T�C�Y
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());

	// 1���_������̃T�C�Y
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	VertexData* vertexData;
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void GpuParticle::CreatePerViewResource()
{
	perViewResource_ = dxCommon_->CreateBufferResource(sizeof(PerView));
	perView_ = nullptr;
	perViewResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&perView_));
}

void GpuParticle::CreateMaterialResource()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Particle::Material));
	material_ = nullptr;
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&material_));
}

void GpuParticle::CreateEmitterResource()
{
	emitterResource_ = dxCommon_->CreateBufferResource(sizeof(Particle::EmitterSphere));
	emitter_ = nullptr;
	emitterResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&emitter_));
	emitter_->emit = 0;
	emitter_->frequency = 0.0f;
	emitter_->frequencyTime = 0.0f;
	emitter_->count = 512;
}

void GpuParticle::CreatePerFrameResource()
{
	perFrameResource_ = dxCommon_->CreateBufferResource(sizeof(Particle::PerFrame));
	perFrame_ = nullptr;
	perFrameResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));
}

void GpuParticle::CreateTransformResource()
{
	transformResource_ = dxCommon_->CreateBufferResource(sizeof(Particle::Transform));
	transform_ = nullptr;
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transform_));
}

void GpuParticle::CreateLineSegmentResource()
{
	lineSegmentResource_ = dxCommon_->CreateBufferResource(sizeof(Particle::LineSegment) * lineSegments_.size());
	Particle::LineSegment* lineSegmentData;
	lineSegmentResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&lineSegmentData));
	std::memcpy(lineSegmentData, lineSegments_.data(), sizeof(Particle::LineSegment) * lineSegments_.size());

	lineSegmentSrvIndex_ = srvManager_->Allocate();

	srvManager_->CreateSRVforStructuredBuffer(lineSegmentSrvIndex_, lineSegmentResource_.Get(), static_cast<UINT>(lineSegments_.size()), sizeof(Particle::LineSegment));
}

void GpuParticle::CreateLineCountResource()
{
	lineCountResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t));
	lineCount_ = nullptr;
	lineCountResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&lineCount_));

	*lineCount_ = static_cast<uint32_t>(lineSegments_.size());
}

void GpuParticle::CreateInitializePipelineSet()
{
	initializePipelineSet_ = std::make_unique<PipelineSet>();

	HRESULT hr;

	// rootSignature�̐���
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameter�̐���
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// �V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// �o�C�i�������RootSignature�𐶐�
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&initializePipelineSet_->rootSignature));

	CreateComputePipelineState(initializePipelineSet_.get(), "Resources/shaders/InitializeParticle.CS.hlsl");
}

void GpuParticle::CreateEmitPipelineSet()
{
	emitPipelineSet_ = std::make_unique<PipelineSet>();

	HRESULT hr;

	// rootSignature�̐���
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameter�̐���
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 0;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 1;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// �V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// �o�C�i�������RootSignature�𐶐�
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&emitPipelineSet_->rootSignature));

	CreateComputePipelineState(emitPipelineSet_.get(), "Resources/shaders/EmitParticleSphere.CS.hlsl");
}

void GpuParticle::CreateLineEmitPipelineSet()
{
	lineEmitPipelineSet_ = std::make_unique<PipelineSet>();
	
	HRESULT hr;

	// rootSignature�̐���
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// rootParameter�̐���
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 0;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].Descriptor.ShaderRegister = 2;

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[5].Descriptor.ShaderRegister = 1;

	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[6].Descriptor.ShaderRegister = 2;

	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[7].Descriptor.ShaderRegister = 3;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// �V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// �o�C�i�������RootSignature�𐶐�
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&lineEmitPipelineSet_->rootSignature));

	CreateComputePipelineState(lineEmitPipelineSet_.get(), "Resources/shaders/EmitParticleLine.CS.hlsl");
}

void GpuParticle::CreateUpdatePipelineSet()
{
	updatePipelineSet_ = std::make_unique<PipelineSet>();

	HRESULT hr;

	// rootSignature�̐���
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// rootParameter�̐���
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 2;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 0;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// �V���A���C�Y���ăo�C�i���ɂ���
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	// �o�C�i�������RootSignature�𐶐�
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&updatePipelineSet_->rootSignature));

	CreateComputePipelineState(updatePipelineSet_.get(), "Resources/shaders/UpdateParticle.CS.hlsl");
}

void GpuParticle::CreateGraphicsRootSignature()
{
	graphicsPipelineSet_ = std::make_unique<PipelineSet>();

	HRESULT hr;

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
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&graphicsPipelineSet_->rootSignature));
	assert(SUCCEEDED(hr));
}

void GpuParticle::CreateGraphicsPipelineState()
{
	HRESULT hr;

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
	graphicsPipelineStateDesc.pRootSignature = graphicsPipelineSet_->rootSignature.Get();
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
	graphicsPipelineSet_->pipelineState = nullptr;
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineSet_->pipelineState));
	assert(SUCCEEDED(hr));
}

void GpuParticle::CreateComputePipelineState(PipelineSet* pipelineSet, std::string shaderPath)
{
	std::wstring wShaderPath(shaderPath.begin(), shaderPath.end());

	Microsoft::WRL::ComPtr<IDxcBlob> computeShaderBlob = dxCommon_->CompileShader(wShaderPath.c_str(), L"cs_6_0");

	// Shader�̃R���p�C��
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = { .pShaderBytecode = computeShaderBlob->GetBufferPointer(), .BytecodeLength = computeShaderBlob->GetBufferSize() };

	// RootSignature�̐ݒ�
	computePipelineStateDesc.pRootSignature = pipelineSet->rootSignature.Get();

	// ���ۂɐ���
	HRESULT hr = dxCommon_->GetDevice()->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&pipelineSet->pipelineState));
}

void GpuParticle::CreatePipelineSet()
{
	/// ComputeShader

	// InitializeParticle�p
	CreateInitializePipelineSet();

	// EmitParticle�p
	CreateEmitPipelineSet();

	CreateLineEmitPipelineSet();

	// UpdateParticle
	CreateUpdatePipelineSet();

	// �`��p
	CreateGraphicsRootSignature();
	CreateGraphicsPipelineState();
}
