#pragma once
#include "Logger.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "Vectors.h"
#include "Matrixs.h"
#include "SrvManager.h"
#include "Camera.h"
#include <random>
#include <array>


class ParticleManager
{

public:

	enum class BlendMode
	{
		kNone,
		kNormal,
		kAdd,
		kSubtract,
		kMultiply,
		kScreen,

		kCountOfBlendMode,
	};

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct Material {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};


	struct MaterialData
	{
		std::string textureFilePath;
		uint32_t textureIndex;
	};

	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	struct AccelerationField {
		Vector3 acceleration; // 加速度
		AABB area; // 範囲
	};

	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	struct ParticleGroup {
		MaterialData materialData;
		std::list<Particle> particles;
		uint32_t srvIndex;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
		uint32_t kNumInstance;
		ParticleForGPU* instancingData;
	};

public:
	static ParticleManager* GetInstance();

	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager,Camera* camera);

	void Update();

	void Draw(std::string filePath);
	
	// パーティクルグループの生成関数
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

	void Emit(const std::string name, const Vector3& position, uint32_t count);

	std::unordered_map<std::string, ParticleGroup> GetParticleGroups() { return particleGroups; }

	void Finalize();

	// particleGroupsをリセット
	void ResetParticleGroups() {
		for (auto& group : particleGroups) {
			// MaterialDataのリセット
			group.second.materialData = MaterialData();  // MaterialDataの初期化

			// パーティクルのリセット
			for (auto& particle : group.second.particles) {
				// Transformのリセット
				particle.transform = Transform();  // Transformの初期化

				// 速度、色、ライフタイムのリセット
				particle.velocity = Vector3{ 0.0f, 0.0f, 0.0f };
				particle.color = Vector4{ 0.0f, 0.0f, 0.0f, 0.0f };  // 色を初期化（例として黒）
				particle.lifeTime = 0.0f;
				particle.currentTime = 0.0f;
			}

			// Instancing関連のリセット
			group.second.srvIndex = 0;
			group.second.instancingResource.Reset();  // Resetでリセット
			group.second.kNumInstance = 0;
			group.second.instancingData = nullptr;  // インスタンシングデータのポインタをnullptrに設定
		}
	}

	void SetBlendMode(std::string sBlendMode);

private:
	static std::unique_ptr<ParticleManager> instance;

	friend std::unique_ptr<ParticleManager> std::make_unique<ParticleManager>();
	friend std::default_delete<ParticleManager>;

	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(ParticleManager&) = delete;
	ParticleManager& operator= (ParticleManager&) = delete;

	// シグネチャの生成関数
	void CreateRootSignature();

	// パイプラインの生成関数
	void CreatePipeline();

	// 頂点データの初期化関数
	void InitializeVertexData();

	// 頂点リソースの生成関数
	void CreateVertexResource();

	// 頂点バッファビューの生成関数
	void CreateVertexBufferView();

	// 頂点リソースに頂点データを書き込む関数
	void WriteDataInResource();

	void calculationBillboardMatrix();
	
	Particle MakeNewParticle(const Vector3& translate);



private:




	Matrix4x4 billboardMatrix = MakeIdentity4x4();

	AccelerationField accelerationField;

	DirectXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;
	Camera* camera_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, size_t(BlendMode::kCountOfBlendMode)> sPipeLineStates_;

	BlendMode blendMode = BlendMode::kAdd;

	ModelData modelData;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;

	std::unordered_map<std::string, ParticleGroup> particleGroups;

	const uint32_t kNumMaxInstance = 100;

	std::mt19937 randomEngine;

private:
	std::unique_ptr<uint32_t> indexData;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;


};

