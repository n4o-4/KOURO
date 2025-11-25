#pragma once
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "Vectors.h"

#include "ModelDatas.h"
#include "ViewProjection.h"

struct ParticleCS
{
	Vector3 translate; // 位置	
	Vector3 scale;     // 
	float lifeTime;    // 
	Vector3 velocity;  //
	float currentTime; // 
	Vector4 color;     // 
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

namespace Particle
{
	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct EmitterSphere
	{
		Vector3 translate;   // 位置
		float radius;        // 射出半径
		uint32_t count;      // 射出数
		float frequency;     // 射出間隔
		float frequencyTime; // 射出間隔調整用時間
		uint32_t emit;       // 射出許可
	};

	struct PerFrame
	{
		float time;
		float deltaTime;
	};
}

class GpuParticle
{
public:

	// 最大パーティクル数
	const uint32_t kMaxParticleCount = 1024;

public:

	static GpuParticle* GetInstance();

	/**
	* \brief  Initialize 初期化
	* \param  dxCommon DirectXCommonのポインタ
	* \param  srvManager SrvManagerのポインタ
	* \param  uavManagedr UavManagerのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager,UavManager* uavManagedr);

	// \brief  Update 更新
	void Update(ViewProjection viewProjection);

	void Draw();

	void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; }

private: // 非公開メンバ関数

	// \brief  CreateResourc0e リソースの生成
	void CreateResource();

	// \brief  CreateVertexResource 頂点リソースの生成
	void CreateVertexResource();

	// \brief  CreateVertexBufferView 頂点バッファビューの生成
	void CreateVertexBufferView();

	// \brief  CreatePerViewResource パービュ―リソースの生成
	void CreatePerViewResource();

	// \brief  CreateMaterialResource マテリアルリソースの生成
	void CreateMaterialResource();

	// \brief  CreateEmitterResource エミッターリソースの生成
	void CreateEmitterResource();

	// \brief  CreatePerFrameResource perFrameリソースの生成
	void CreatePerFrameResource();

	/// Compute

	// initialize
	void CreateInitializePipelineSet();

	// emit
	void CreateEmitPipelineSet();

	// update
	void CreateUpdatePipelineSet();

	/// Graphics

	// \brief  CreateGraphicsRootSignature 描画用ルートシグネチャの作成
	void CreateGraphicsRootSignature();

	// \brief  CreateGraphicsPipelineState 描画用パイプラインステートの作成
	void CreateGraphicsPipelineState();

	// \brief  CreateComputePipelineState Computeパイプラインステートの作成
	void CreateComputePipelineState(PipelineSet* pipelineSet,std::string shaderPath);

	// \brief  CreatePipelineSet パイプラインセットの作成
	void CreatePipelineSet();

private:

	static std::unique_ptr<GpuParticle> instance;

	friend std::unique_ptr<GpuParticle> std::make_unique<GpuParticle>();
	friend std::default_delete<GpuParticle>;

	GpuParticle() = default;
	~GpuParticle() = default;
	GpuParticle(GpuParticle&) = delete;
	GpuParticle& operator= (GpuParticle&) = delete;


	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	UavManager* uavManager_ = nullptr;

	MaterialData materialData_;

	ParticleModel::ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> particleResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> counterResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_ = nullptr;
	PerView* perView_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Particle::Material* material_ = nullptr;

	// エミッター
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
	Particle::EmitterSphere* emitter_ = nullptr;

	// perFrame
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_ = nullptr;
	Particle::PerFrame* perFrame_ = nullptr;

	uint32_t srvIndex_ = 0;

	uint32_t uavIndex_ = 0;

	// ComputeShader用(InitializeParticle)
	std::unique_ptr<PipelineSet> initializePipelineSet_;
	
	// ComputeShader用(EmitParticle)
	std::unique_ptr<PipelineSet> emitPipelineSet_;

	// ComputeShader用(UpdateParticle)
	std::unique_ptr<PipelineSet> updatePipelineSet_;

	// 描画用
	std::unique_ptr<PipelineSet> graphicsPipelineSet_;

	

	const float kDeltaTime_ = 1.0f / 60.0f;
};

