#include "GpuParticleManager.h"

GpuParticleManager::GpuParticleManager(EngineContext context)
{
	device_ = context.device;

	commandList_ = context.commandList;
}

void GpuParticleManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	CreateResources();

	ParticleInitialize();
};

void GpuParticleManager::Update()
{
}

void GpuParticleManager::ParticleInitialize()
{
	// RootSignatureの設定（initialize用）
	dxCommon_->GetCommandList()->SetComputeRootSignature(pipelineSets_.find("initialize")->second.get()->rootSignature.Get());

	// PipelineStateの設定（initialize用）
	dxCommon_->GetCommandList()->SetPipelineState(pipelineSets_["initialize"]->pipelineState.Get());

	// UAVの設定（u0: パーティクル, u1: カウンタ, u2: フリーリスト）
	//dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(0, particleResource_.Get()->GetGPUVirtualAddress());
	//dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(1, counterResource_.Get()->GetGPUVirtualAddress());
	//dxCommon_->GetCommandList()->SetComputeRootUnorderedAccessView(2, freeListResource_.Get()->GetGPUVirtualAddress());

	// ComputeShaderの実行
	dxCommon_->GetCommandList()->Dispatch(1, 1, 1);
}

void GpuParticleManager::CreateResources()
{
	//// パーティクルのリソース生成
	//particleResource_ = dxCommon_->CreateComputeBufferResource(sizeof(Particle) * kMaxParticleCount);

	//// カウンターのリソース生成
	//counterResource_ = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t));

	//// フリーリストのリソース生成
	//freeListResource_ = dxCommon_->CreateComputeBufferResource(sizeof(uint32_t) * kMaxParticleCount);
}

void GpuParticleManager::CreateInitializePipelineSet()
{
	// pipelineSetの生成（Compute用のパイプラインセットを確保）
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignatureの記述構造体を初期化
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// Input Assembler（頂点入力レイアウト）を許可するフラグを設定
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter配列（UAVを3つ使用する）
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

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

void GpuParticleManager::CreateUpdatePipelineSet()
{
	// 新しい Compute 用の PipelineSet を生成
	std::unique_ptr<PipelineSet> pSet = std::make_unique<PipelineSet>();

	// RootSignature の記述構造体を初期化
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// Input Assembler（頂点入力レイアウト）の使用を許可するフラグを設定
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter 配列を初期化（UAV×3 + CBV×1）
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

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

	// CBV レジスタ 0 番を設定（定数バッファ：パーティクル全体の共通データなどに使用）
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].Descriptor.ShaderRegister = 0;

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
	CreateComputePipelineState(pSet.get(), "Resources/shaders/UpdateParticle.CS.hlsl");

	// unordered_map に追加
	pipelineSets_["update"] = std::move(pSet);
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
};
