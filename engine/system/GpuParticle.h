#pragma once
#include "D3D12Context.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "Vectors.h"

#include "ModelDatas.h"
#include "ViewProjection.h"

#include "LineModelManager.h"

namespace Kouro
{
	struct ParticleCS
	{
		Vector3 translate; float pad0;
		Vector3 scale;     float pad1;
		float lifeTime;    float pad2[3];
		Vector3 velocity;  float pad3;
		float currentTime; float pad4[3];
		Vector4 color;
	};

	struct PerView
	{
		Matrix4x4 viewProjection;
		Matrix4x4 billboardMatrix;
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
			Vector3 translate;   //!< 球の中心座標
			float radius;        //!< 発射する半径
			uint32_t count;      //!< 発射する数
			float frequency;     //!< 発射間隔
			float frequencyTime; //!< 発射タイマー
			uint32_t emit;       //!< 発射フラグ
		};

		struct PerFrame
		{
			float time;      //!< 現在の時間
			float deltaTime; //!< 前フレームからの経過時間
		};

		struct Transform
		{
			Matrix4x4 matWorld; //!< ワールド行列
		};

		struct LineSegment
		{
			Vector3 start;     //!< 線分の始点
			float pad1 = 0.0f;
			Vector3 end;       //!< 線分の終点
			float pad2 = 0.0f;
		};
	}

	class GpuParticle
	{
	public:

		const uint32_t kMaxParticleCount = 524288; //!< 最大パーティクル数

	public:

		/// \brief  シングルトンインスタンスの取得
		static GpuParticle* GetInstance();

		/**
		* \brief  初期化
		* \param  dxCommon DirectXCommonのポインタ
		* \param  srvManager SrvManagerのポインタ
		* \param  uavManagedr UavManagerのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, UavManager* uavManagedr);

		// \brief  終了
		void Finalize();

		/// \brief  更新
		void Update(ViewProjection viewProjection);

		/// \brief  描画
		void Draw();

		/**
		* \brief
		* \param  time      : 現在の時間
		* \param  deltaTime : 前フレームからの経
		*/
		void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; }

		/**
		* \brief  線分を作成する
		* \param  filePath : 線分データのファイルパス
		*/
		void CreateLineSegment(std::string filePath);

		/// \brief  線分を発生させる
		void LineEmit(std::string groupName, Matrix4x4 world);

	private: // 

		/// \brief particleリソースの作成
		void CreateResource();

		/// \brief 頂点リソースの作成
		void CreateVertexResource();

		/// \brief 頂点バッファビューの作成
		void CreateVertexBufferView();

		/// \brief perViewリソースの作成
		void CreatePerViewResource();

		/// \brief materialリソースの作成
		void CreateMaterialResource();

		/// \brief Emitterリソースの作成
		void CreateEmitterResource();

		/// \brief perFrameリソースの作成
		void CreatePerFrameResource();

		/// \brief transformリソースの作成
		void CreateTransformResource();

		/// \brief 線分用リソースの作成
		void CreateLineSegmentResource();

		/// \brief  線分カウント用リソースの作成
		void CreateLineCountResource();

		/// --------------- コンピュートシェーダー用

		/// \brief  初期化用のPipelineSetを作成
		void CreateInitializePipelineSet();

		/// \brief  発生用のPipelineSetを作成
		void CreateEmitPipelineSet();

		/// \brief  線分発生用のPipelineSetを作成
		void CreateLineEmitPipelineSet();

		/// \brief  更新用のPipelineSetを作成
		void CreateUpdatePipelineSet();

		/// --------------- 描画用

		/// \brief 描画用ルートシグネチャの作成
		void CreateGraphicsRootSignature();

		/// \brief  
		void CreateGraphicsPipelineState();

		/**
		* \brief  ComputePipelineStateの作成
		* \param  pipelineSet : PipelineSetのポインタ
		* \param  shaderPath  : シェーダーファイルのパス
		*/
		void CreateComputePipelineState(PipelineSet* pipelineSet, std::string shaderPath);

		/// \brief  パイプラインセットの作成
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

		// �G�~�b�^�[
		Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
		Particle::EmitterSphere* emitter_ = nullptr;

		// perFrame
		Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_ = nullptr;
		Particle::PerFrame* perFrame_ = nullptr;

		// transform
		Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_ = nullptr;
		Particle::Transform* transform_ = nullptr;

		// lineSegment
		Microsoft::WRL::ComPtr<ID3D12Resource> lineSegmentResource_ = nullptr;
		Particle::LineSegment* lineSegment_ = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> lineCountResource_ = nullptr;
		uint32_t* lineCount_ = nullptr;

		uint32_t srvIndex_ = 0;

		uint32_t uavIndex_ = 0;


		std::unique_ptr<PipelineSet> initializePipelineSet_;


		std::unique_ptr<PipelineSet> emitPipelineSet_;


		std::unique_ptr<PipelineSet> lineEmitPipelineSet_;


		std::unique_ptr<PipelineSet> updatePipelineSet_;


		std::unique_ptr<PipelineSet> graphicsPipelineSet_;

		const float kDeltaTime_ = 1.0f / 60.0f;

		std::vector<Particle::LineSegment> lineSegments_;

		uint32_t lineSegmentSrvIndex_ = 0;

	};
}