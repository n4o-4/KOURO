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

		Microsoft::WRL::ComPtr<ID3D12Resource> baseUpdateListResource = nullptr; // lifeTimeやtrasnformにvelocityを加算
		Microsoft::WRL::ComPtr<ID3D12Resource> noiseUpdateListResource = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
		GpuParticleManager::Material* material = nullptr;

		//std::vector<uint32_t> textureIndexList;
		uint32_t particleSrvIndex = 0;
		uint32_t particleUavIndex = 0;
		uint32_t textureIndex = 0;
	};

public:
	
	/**
	* \brief  コンストラクタ
	* \param  context    : 
	* \param  srvManager : SrvManagerのポインタ
	*/
	GpuParticleManager(EngineContext context,SrvManager* srvManager);

	/// \brief 初期化
	void Initialize();

	/// \brief 更新
	void Update();

	/**
	* \brief  描画
	* \param  viewPro : ViewProjectionのポインタ
	*/
	void Draw(ViewProjection* viewPro);

	void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; };

	void ParticleInitialize(ParticleGroup group);

	void CreateParticleGroup(const std::string name,const std::string textureFilePath, std::vector<VertexData> vertices);

private:


	void CreateResources();

	void CreatePipelineSets();

	/// \brief パーティクルの初期化用のCSPipelineSet作成関数
	void CreateInitializePipelineSet();

	/// \brief パーティクル汎用更新のCSPipelineSet作成関数
	void CreateBaseUpdatePipelineSet();

	/// \brief ノイズが入った動きのCSPipelineSet生成関数
	void CreateNoiseUpdatePipelineSet();

	/**
	* \brief  ComputeShader　用の pipelineState 汎用作成関数
	* \param  pipelineSet : 生成先の pipelineSet のポインタ
	* \param  csFileName  : 使用するComputeShaderのファイル名
	*/
	void CreateComputePipelineState(PipelineSet* pipelineSet, std::string csFileName);

	void CreateGraphicsPipelineSet();

private: //!< メンバ変数

	HRESULT hr = {}; //!< 

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	UavManager* uavManager_ = nullptr;

	ID3D12Device* device_ = nullptr;

	ID3D12CommandList* commandList_ = nullptr;

	std::unordered_map<std::string, std::unique_ptr<PipelineSet>> pipelineSets_ = {}; //!< パイプラインを名前で管理するマップ

	std::unordered_map<std::string, ParticleGroup> particleGroups_ = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
	EmitterSphere* emitter_ = nullptr;

	// perFrame
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_ = nullptr;
	PerFrame* perFrame_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_ = nullptr;
	PerView* perView_ = nullptr;

	// transform
	Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_ = nullptr;
	Transform* transform_ = nullptr;
};