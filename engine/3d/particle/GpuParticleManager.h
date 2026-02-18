#pragma once

#include "DirectXCommon.h"
#include "Vectors.h"

#include "ModelDatas.h"
#include "ViewProjection.h"

#include "LineModelManager.h"

#include "GpuContext.h"

namespace Kouro
{
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
		};

		struct EmitterSphere
		{
			Vector3 translate;   // 
			float radius;        // 
			float frequency;     // 
			float frequencyTime; // 
			float pad1[2];
			uint32_t count;      // 
			uint32_t emit;       // 
			uint32_t pad2[2];
		};

		struct EmitterSphereShell
		{
			Vector3 translate;
			float minRadius;
			float maxRadius;

			uint32_t emit;
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

	public:

		struct ParticleGroup
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> particleResource = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> counterResource = nullptr;
			Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource = nullptr;

			Microsoft::WRL::ComPtr<ID3D12Resource> baseUpdateListResource = nullptr; // lifeTimeやtransformにvelocityを加算
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
		GpuParticleManager(GpuContext context);

		/// \brief 初期化
		void Initialize();

		/// \brief 更新
		void Update();

		/**
		* \brief  描画
		* \param  viewPro : ViewProjectionのポインタ
		*/
		void Draw(ViewProjection viewPro);

		/**
		* \brief  指定したグループのパーティクルを初期化
		* \param  group : 指定するパーティクルのグループ
		*/
		void ParticleInitialize(ParticleGroup group);

		/**
		* \brief  パーティクルグループの作成とparticleGrous_への追加
		* \param  name            : particleGrous_に追加する際のキー
		* \param  textureFilePath : 使用するテクスチャのパス
		* \param  vertices        : 使用するパーティクル形状の頂点
		*/
		void CreateParticleGroup(const std::string name, const std::string textureFilePath, std::vector<VertexData> vertices);

		/**
		* \brief  ライン上にパーティクルを放出する
		* \param  groupName     : パーティクルグループの名前
		* \param  lineSrvIndex  : ラインモデルのSRVインデックス
		* \param  lineCount     : ラインモデルのライン数
		* \param  emitterResource : エミッター情報のリソース
		* \param  world         : ラインモデルのワールド行列
		*/
		void LineEmit(std::string groupName, uint32_t lineSrvIndex, uint32_t lineCount, Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource, Matrix4x4 world);

		/**
		* \brief  点からパーティクルを放出する
		* \param  groupName     : パーティクルグループの名前
		* \param  emitterResource : エミッター情報のリソース
		* \param  world         : ワールド行列
		*/
		void PointEmit(std::string groupName, ID3D12Resource* emitterResource, ID3D12Resource* transformResource);

		/**
		* \brief  perFrameの値を設定する
		* \param  time      : 合計時間
		* \param  deltaTime : 前フレームからの経過時間
		*/
		void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; };

		/**
		* \brief  particleGroups_を取得する
		* \return particleGroups_の参照
		*/
		std::unordered_map<std::string, ParticleGroup>& GetParticleGroups() { return particleGroups_; }

		/**
		* \brief  perFrameResource_を取得する
		* \return perFrameResource_
		*/
		const Microsoft::WRL::ComPtr<ID3D12Resource> GetPerFrameResource() const { return perFrameResource_; }

	private:

		/// \brief リソース作成関数
		void CreateResources();

		/// \brief パイプラインセット作成関数
		void CreatePipelineSets();

		/// \brief パーティクルの初期化用のCSPipelineSet作成関数
		void CreateInitializePipelineSet();

		/// \brief パーティクル汎用更新のCSPipelineSet作成関数
		void CreateBaseUpdatePipelineSet();

		/// \brief ノイズが入った動きのCSPipelineSet生成関数
		void CreateNoiseUpdatePipelineSet();

		/// \brief モデルエッジエミッター用のCSPipelineSet作成関数
		void CreateModelEdgeEmitterPipelineSet();

		/// \brief 球体シェルエミッター用のCSPipelineSet作成関数
		void CreateSphereShellEmitterPipelineSet();

		/// \brief 点エミッター用のCSPipelineSet作成関数
		void CreatePointEmitterPipelineSet();

		/**
		* \brief  ComputeShader　用の pipelineState 汎用作成関数
		* \param  pipelineSet : 生成先の pipelineSet のポインタ
		* \param  csFileName  : 使用するComputeShaderのファイル名
		*/
		void CreateComputePipelineState(PipelineSet* pipelineSet, std::string csFileName);

		/// \brief グラフィックスパイプラインセット作成関数
		void CreateGraphicsPipelineSet();

	private: //!< メンバ変数

		HRESULT hr = {}; //!< 

		DirectXCommon* dxCommon_ = nullptr;

		SrvManager* srvManager_ = nullptr;

		UavManager* uavManager_ = nullptr;

		ID3D12Device* device_ = nullptr;

		ID3D12GraphicsCommandList* commandList_ = nullptr;

		GpuResourceUtils* gpuResourceUtils_ = nullptr;

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

		Microsoft::WRL::ComPtr<ID3D12Resource> emitterSphereShellResource_ = nullptr;

		static constexpr float kMaxRadius = 100000.0f; //!< エミッターの最大半径

		static constexpr float kMinRadius = 70000.0f; //!< エミッターの最小半径
	};
}