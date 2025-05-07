#include "ColorSpace.h"

void ColorSpace::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	// �p�C�v���C���̐���
	BaseEffect::Initialize(dxCommon, srvManager);

	//�p�C�v���C���̏�����
	CreatePipeline();

	// �}�e���A���̐���
	CreateMaterial();

	hue = 0.0f;
	saturation = 0.0f;
	value = 0.0f;
}

void ColorSpace::Update()
{
	data_->hue = hue;
	data_->saturation = saturation;	
	data_->value = value;

#ifdef _DEBUG

	DrawImGui();

#endif
}

void ColorSpace::Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex)
{
	// �`����RTV�̃C���f�b�N�X
	uint32_t renderTextureIndex = 2 + renderTargetIndex;

	// �`����RTV��ݒ肷��
	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &*dxCommon_->GetRTVHandle(renderTextureIndex), false, nullptr);

	// ���[�g�V�O�l�`���̐ݒ�	
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(pipeline_.get()->rootSignature.Get());

	// �p�C�v���C���X�e�[�g�̐ݒ�
	dxCommon_->GetCommandList()->SetPipelineState(pipeline_.get()->pipelineState.Get());

	// renderTexture��SrvHandle���擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = (renderResourceIndex == 0) ? TextureManager::GetInstance()->GetSrvHandleGPU("RenderTexture0") : TextureManager::GetInstance()->GetSrvHandleGPU("RenderTexture1");

	// SRV��ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvHandle);

	// Cbuffer�̐ݒ�
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, resource_.Get()->GetGPUVirtualAddress());

	// �`��
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void ColorSpace::CreatePipeline()
{
	// ���[�g�V�O�l�`���̐���
	CreateRootSignature(pipeline_.get());

	// �p�C�v���C���X�e�[�g�̐���
	CreatePipeLineState(pipeline_.get());
}

void ColorSpace::CreateRootSignature(Pipeline* pipeline)
{
	HRESULT hr;

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// SRV �� Descriptor Range
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // t0: Shader Register
	descriptorRange[0].NumDescriptors = 1; // 1��SRV
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // �����v�Z

	// Root Parameter: SRV (gTexture)
	D3D12_ROOT_PARAMETER rootParameters[2] = {};

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // Pixel Shader�Ŏg�p
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange; // Table�̒��g�̔z����w��
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Table�ŗ��p���鐔

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBV���g��
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  //PixelShader�Ŏg��
	rootParameters[1].Descriptor.ShaderRegister = 0;    // ���W�X�^�ԍ�0�ƃo�C���h
	// Static Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // �o�C���j�A�t�B���^
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // �SMipMap�g�p
	staticSamplers[0].ShaderRegister = 0; // s0: Shader Register
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // Pixel Shader�Ŏg�p

	// ���[�g�V�O�l�`���̍\�z
	descriptionRootSignature.pParameters = rootParameters; // ���[�g�p�����[�^�[�z��ւ̃|�C���^
	descriptionRootSignature.NumParameters = _countof(rootParameters); // �z��̒���
	descriptionRootSignature.pStaticSamplers = staticSamplers; // �T���v���[�z��ւ̃|�C���^
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers); // �T���v���[�̐�

	//�V���A���C�Y���ăo�C�i���ɂ���

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// �o�C�i�������ɐ���
	pipeline->rootSignature = nullptr;
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline->rootSignature));
	assert(SUCCEEDED(hr));
}

void ColorSpace::CreatePipeLineState(Pipeline* pipeline)
{
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};

	// ���ׂĂ̗v�f������������
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// ����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;

	// �O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shader���R���p�C������
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Fullscreen.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/ColorSpace.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// DepthStencilState�̐ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depth�̋@�\��L��������
	depthStencilDesc.DepthEnable = false;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pipeline->rootSignature.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;  // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };   // PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizerState

	// ��������RTV�̏��
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// ���p����g�|���W(�`��)�̃^�C�v.�B�O�p�`
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �ǂ̂悤�ɉ�ʂɐF�����邩
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencil�̐ݒ�
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ���ۂɐ���
	pipeline->pipelineState = nullptr;
	dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipeline->pipelineState));
}

void ColorSpace::CreateMaterial()
{
	// bufferResource�̐���
	resource_ = dxCommon_->CreateBufferResource(sizeof(ColorSpaceShader::Material));

	// �f�[�^���}�b�v
	resource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&data_));
}

void ColorSpace::DrawImGui()
{
	if (ImGui::TreeNode("colorSpace")) {

		// �����������������ɒ������ڂ�ǉ�������������

    ImGui::DragFloat("hue", &hue,0.01f);
	ImGui::DragFloat("saturation", &saturation, 0.01f);
	ImGui::DragFloat("value", &value, 0.01f);
	
		ImGui::TreePop();
	}
}
