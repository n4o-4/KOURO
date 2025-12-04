#pragma once

#include "DirectXCommon.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "Vectors.h"

#include "ModelDatas.h"
#include "ViewProjection.h"

#include "LineModelManager.h"

class GpuParticleManager
{
private:

	const uint32_t kMaxParticleCount = 524288 * 2; //!< 最大パーティクル数

	const uint32_t kCSMaxParticleCount = 1024;


private:

	struct Particle
	{
		Vector3 translate; float pad0;
		Vector3 scale;     float pad1;
		float lifeTime;    float pad2[3];
		Vector3 velocity;  float pad3;
		float currentTime; float pad4[3];
		Vector4 color;
		uint32_t textureIndex;
	};

	struct EmitterSphere
	{
		Vector3 translate;   // 
		float radius;        // 
		uint32_t count;      // 
		float frequency;     // 
		float frequencyTime; // 
		uint32_t emit;       // 
	};

	struct PerFrame
	{
		float time;
		float deltaTime;
	};

	struct Transform
	{
		Matrix4x4 matWorld;
	};

	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct LineSegment
	{
		Vector3 start;
		float pad1 = 0.0f;
		Vector3 end;
		float pad2 = 0.0f;
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

	struct ParticleGroup
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> particleResource = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> counterResource = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
		GpuParticleManager::Material* material = nullptr;

		std::vector<uint32_t> textureIndices;
	};

public:

	GpuParticleManager(EngineContext context);

	void Initialize();

	void Update();

	void Draw();


	void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; };

	void ParticleInitialize();

private:


	void CreateResources();

	/// \brief パーティクルの初期化用のCSPipelineSet作成関数
	void CreateInitializePipelineSet();

	/// \brief パーティクル汎用更新のCSPipelineSet作成関数
	void CreateUpdatePipelineSet();

	/**
	* \brief  ComputeShader　用の pipelineState 汎用作成関数
	* \param  pipelineSet : 生成先の pipelineSet のポインタ
	* \param  csFileName  : 使用するComputeShaderのファイル名
	*/
	void CreateComputePipelineState(PipelineSet* pipelineSet, std::string csFileName);

private: //!< メンバ変数

	HRESULT hr = {}; //!< 

	DirectXCommon* dxCommon_ = nullptr;

	ID3D12Device* device_ = nullptr;

	ID3D12CommandList* commandList_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<PipelineSet>> pipelineSets_ = {}; //!< パイプラインを名前で管理するマップ

	//Microsoft::WRL::ComPtr<ID3D12Resource> particleResource_ = nullptr;
	//Microsoft::WRL::ComPtr<ID3D12Resource> counterResource_ = nullptr;
	//Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
	EmitterSphere* emitter_ = nullptr;

	// perFrame
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_ = nullptr;
	PerFrame* perFrame_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_ = nullptr;
	PerView* perView_ = nullptr;

};