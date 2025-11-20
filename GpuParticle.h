#pragma once
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "Vectors.h"

struct ParticleCS
{
	Vector3 translate;
	Vector3 scale;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

struct PerView
{
	Matrix4x4 viewProjection;
	Matrix4x4 billboardMatrix;
};

struct PipelineSet
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
};

class GpuParticle
{
public:

	// 最大パーティクル数
	const uint32_t kMaxParticleCount = 1024;

public:

	/**
	* \brief  Initialize 初期化
	* \param  dxCommon DirectXCommonのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager,UavManager uavManagedr);

	void Update();

private:

	/**
	* \brief  CreateResource リソースの生成
	*/
	void CreateResource();

	/**
	* \brief  CrateCmputeRootSignature Compute用ルートシグネチャの作成
	*/
	void CrateComputeRootSignature();

	/**
	* \brief  CreateComputePipelineState Computeパイプラインステートの作成
	*/
	void CreateComputePipelineState();


	void CreateGraphicsRootSignature();

	void CreateGraphicsPipelineState();

	/**
	*/
	void CreatePipelineSet();
private:

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	UavManager* uavManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> particleResource_ = nullptr;

	uint32_t srvIndex_ = 0;

	uint32_t uavIndex_ = 0;

	// ComputeShader用
	PipelineSet computePipelineSet_;

	// 描画用
	PipelineSet graphicsPipelineSet_;
};

