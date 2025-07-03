#include "Skybox.h"

void Skybox::Initialize(const std::string& filePath)
{
	TextureManager::GetInstance()->LoadTexture(filePath);

	TextureManager::GetInstance()->GetTextureIndexByFilePath(filePath);


	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 24);

	vertexBufferView.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * 24);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// �E��
	vertexData_[0].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[1].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[3].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	// ����
	vertexData_[4].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[5].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[7].position = { -1.0f, -1.0f,  1.0f, 1.0f };

	// �O��
	vertexData_[8].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[9].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[10].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertexData_[11].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	// ���
	vertexData_[12].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[13].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[14].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[15].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	// ���
	vertexData_[16].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[17].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertexData_[18].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertexData_[19].position = { 1.0f,  1.0f,  1.0f, 1.0f };

	// ����
	vertexData_[20].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[21].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[22].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertexData_[23].position = { 1.0f, -1.0f,  1.0f, 1.0f };

	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));

	// �������ނ��߂̃A�h���X���擾
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

	CreateGraphicsPipeline();
}

void Skybox::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight.GetDirectionalLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLight.GetPointLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLight.GetSpotLightResource()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);  // VBV��ݒ�

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU());

	
	dxCommon_->GetCommandList()->DrawInstanced(UINT(24), 1, 0, 0);
}

void Skybox::CreateRootSignature()
{
	HRESULT hr;

	// RootSignature�쐬
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0; // 0����͂��܂�
	descriptorRange[0].NumDescriptors = 1; // 1����n�܂�
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV���g��
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offset�������v�Z

	// RootParameter�쐬
	D3D12_ROOT_PARAMETER rootParameters[8] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBV���g��
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  //PixelShader�Ŏg��
	rootParameters[0].Descriptor.ShaderRegister = 0;    // ���W�X�^�ԍ�0�ƃo�C���h

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // Descriptortable���g��
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShader�ōs��
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Table�̒��g�̔z����w��
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Table�ŗ��p���鐔


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

	descriptionRootSignature.pParameters = rootParameters;  // ���[�g�p�����[�^�[�z��ւ̃|�C���^
	descriptionRootSignature.NumParameters = _countof(rootParameters);  // �z��̒���

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // �o�C�i���t�B���^
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1�͈̔͊O�����s�[�g
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // ��r���Ȃ�
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;     // ������MipMap���g��
	staticSamplers[0].ShaderRegister = 0; // ���W�X�^�ԍ�0���g��
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShader�Ŏg��
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//�V���A���C�Y���ăo�C�i���ɂ���

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// �o�C�i�������ɐ���
	rootSignature = nullptr;
	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void Skybox::CreateGraphicsPipeline()
{
	HRESULT hr;

	CreateRootSignature();

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//inputElementDescs[2].SemanticName = "NORMAL";
	//inputElementDescs[2].SemanticIndex = 0;
	//inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendState�̐ݒ�
	D3D12_BLEND_DESC blendDesc{};

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	// ���ׂĂ̗v�f������������
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerState�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// ����(���v���)��\�����Ȃ�
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

	// �O�p�`�̒���h��Ԃ�
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shader���R���p�C������
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Skybox.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/Skybox.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	// DepthStencilState�̐ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depth�̋@�\��L��������
	depthStencilDesc.DepthEnable = true;

	// �������݂��܂�
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	// ��r�֐���LessEqual�B�߂���Ε`�悳���
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get(); // RootSignature
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
	graphicsPipelineState = nullptr;
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

