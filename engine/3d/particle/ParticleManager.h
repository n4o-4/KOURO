#pragma once
#include "Logger.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "Vectors.h"
#include "Matrixs.h"
#include "Camera.h"
#include <random>
#include <array>
#include "CameraManager.h"

#include "ModelManager.h"

#include "EngineContext.h"

// \brief ParticleManager
// CPU側でパーティクルを管理し、GPUにインスタンシングして描画するシステムクラス。
// パーティクルグループの生成、発生、更新、描画、リセットを提供。
// ビルボード、ストレッチ、ブレンドモード、加速度フィールドなどに対応。
// 内部でParticleGroupやParticle構造体を使用して状態を保持し、GPUリソース（頂点バッファ、マテリアル、フラグ、カメラ情報など）を管理。
// シングルトンで提供され、DirectXCommonやSrvManagerとの連携が必要。

namespace Kouro
{
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
			kAlpla,

			kCountOfBlendMode,
		};

		enum class ParticleType
		{
			Normal,
			Ring,
			Cylinder,
		};

		struct ParticleForGPU {
			Vector3 position;  // ワールド座標

			float padding1[1];

			Vector3 scale;

			float padding2[1];

			Vector3 velocity;

			float padding3[1];

			Vector4 color;

			float stretch;

			float padding4[3];
		};

		struct CameraForGPU {
			Matrix4x4 view;
			Matrix4x4 Projection; // ビュー投影行列
			Matrix4x4 billboardMatrix; // ビルボード用の行列

			bool enableBillboard; // ビルボードを有効にするかどうか

			bool enableStretch; // ストレッチを有効にするかどうか

			int padding1[2]; // パディング

			Vector3 cameraPosition; // カメラ位置
			float padding2[1]; // パディング
		};

		struct FlagsForGPU {
			uint32_t enableBillboard; // ビルボードを有効にするかどうか
			uint32_t enableStretch; // ストレッチを有効にするかどうか
			uint32_t padding[2]; // パディング
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
			uint32_t textureIndex = 0;
		};

		/*struct ModelData {
			std::vector<VertexData> vertices;
			MaterialData material;
		};*/

		struct AccelerationField {
			Vector3 acceleration; // 加速度
			AABB area; // 範囲
		};

		struct Particle {
			Transform transform;
			Vector3 startScale;
			Vector3 finishScale;
			Vector3 velocity;
			Vector4 color;
			float lifeTime;
			float currentTime;
			Vector4 startColor;
			Vector4 finishColor;
		};

		struct ParticleGroup
		{
			MaterialData materialData;
			std::list<Particle> particles;
			uint32_t srvIndex;
			Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
			uint32_t kNumInstance;
			ParticleForGPU* instancingData;
			Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
			Material* material;

			ParticleType type;

			BlendMode blendMode = BlendMode::kNone; // ブレンドモード

			bool enableBillboard = false;
			bool sameScale = true;
			bool enableDeceleration = false; // 減速を有効にするかどうか
			float speed = 20.0f; // パーティクルの速度



			bool enablePulse = false; // 輝きを有効にするかどうか

			float stretch = 1.0f; // パーティクルの伸び

			Microsoft::WRL::ComPtr<ID3D12Resource> flagsResource; // フラグ用のリソース
			FlagsForGPU* flagsData; // フラグデータ
		};

		struct Range
		{
			float min;
			float max;
		};

		struct ColorRange
		{
			Vector4 min;
			Vector4 max;
		};

		struct Vec3Range
		{
			Vector3 min;
			Vector3 max;
		};

		struct ParticleStates
		{
			ColorRange startColorRange;     // 発生時の色の範囲
			ColorRange finishColorRange;    // 終了時の色の範囲
			Vec3Range velocityRange;        // 速度の範囲
			Vec3Range rotateRange;          // 回転の範囲
			Vec3Range startScaleRange;      // 発生時のスケールの範囲
			Vec3Range finishScaleRange;     // 終了時のスケールの範囲
			Vec3Range translateRange;       // 発生位置の範囲
			Range lifeTimeRange;            // ライフタイムの範囲
		};

	public:
		/**
		* \brief  シングルトンインスタンスの取得
		* \return ParticleManagerのインスタンス
		*/
		static ParticleManager* GetInstance();

		/**
		* \brief  ParticleManagerの初期化
		* \param  dxCommon DirectXCommonのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

		/// \brief パーティクルの更新
		void Update();

		/**
		* \brief  パーティクルの描画
		* \param  filePath シェーダーファイルのパス
		*/
		void Draw(std::string filePath);

		/**
		* \brief  パーティクルグループの作成
		* \param  name パーティクルグループの名前
		* \param  textureFilePath テクスチャファイルのパス
		* \param  type パーティクルのタイプ
		*/
		void CreateParticleGroup(const std::string name, const std::string textureFilePath, ParticleType type);

		/**
		* \brief  パーティクルの発生
		* \param  name パーティクルグループの名前
		* \param  position 発生位置
		* \param  count 発生数
		* \param  states パーティクルの状態情報
		*/
		void Emit(const std::string name, const Vector3& position, uint32_t count, ParticleStates states);

		/**
		* \brief  放射状にパーティクルを発生させる
		* \param  name パーティクルグループの名前
		* \param  position 発生位置
		* \param  count 発生数
		* \param  states パーティクルの状態情報
		*/
		void RadialEmit(const std::string name, const Vector3& position, uint32_t count, ParticleStates states);

		/**
		* \brief  パーティクルグループの取得
		* \return particleGroups パーティクルグループ群
		*/
		std::unordered_map<std::string, ParticleGroup> GetParticleGroups() { return particleGroups; }

		/// \brief 終了処理
		void Finalize();

		/// \brief パーティクルグループのリセット
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

		/**
		* \brief  CameraManagerのセット
		* \param  cameraManager CameraManagerのポインタ
		*/
		void SetCameraManager(CameraManager* cameraManager) { cameraManager_ = cameraManager; }

		/**
		* \brief  パーティクルグループの取得
		* \param  name パーティクルグループの名前
		*/
		ParticleGroup* GetParticleGroup(const std::string& name) {
			auto it = particleGroups.find(name);
			if (it != particleGroups.end()) {
				return &it->second;
			}
			return nullptr;
		}

	private:
		static std::unique_ptr<ParticleManager> instance;

		friend std::unique_ptr<ParticleManager> std::make_unique<ParticleManager>();
		friend std::default_delete<ParticleManager>;

		ParticleManager() = default;
		~ParticleManager() = default;
		ParticleManager(ParticleManager&) = delete;
		ParticleManager& operator= (ParticleManager&) = delete;

		/// \brief ルートシグネチャの作成
		void CreateRootSignature();

		/// \brief パイプラインステートの作成
		void CreatePipeline();

		///	\brief 頂点データの初期化
		void InitializeVertexData();

		/// \brief 頂点リソースの作成
		void CreateVertexResource();

		/// \brief 頂点バッファビューの作成
		void CreateVertexBufferView();

		/// \brief マテリアルリソースの作成
		void WriteDataInResource();

		/// \brief ビルボード行列の計算
		void calculationBillboardMatrix();

		/**
		* \brief  新しいパーティクルの作成
		* \param  translate 発生位置
		* \param  states パーティクルの状態情報
		* \param  sameScale スケールを同じにするかどうか
		* \param  speed パーティクルの速度
		*/
		Particle MakeNewParticle(const Vector3& translate, ParticleStates states, bool sameScale, float speed);

		/**
		* \brief  新しい放射状パーティクルの作成
		* \param  translate 発生位置
		* \param  states パーティクルの状態情報
		* \param  sameScale スケールを同じにするかどうか
		* \param  speed パーティクルの速度
		*/
		Particle MakeNewRadialParticle(const Vector3& translate, ParticleStates states, bool sameScale, float speed);

	private:

		/**
		* \brief  減速の更新
		* \param  particle パーティクル参照
		*/
		void DecelerationUpdate(Particle& particle);

	private:

		Matrix4x4 billboardMatrix = MakeIdentity4x4();

		AccelerationField accelerationField;

		DirectXCommon* dxCommon_ = nullptr;
		SrvManager* srvManager_ = nullptr;
		CameraManager* cameraManager_ = nullptr;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

		std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, size_t(BlendMode::kCountOfBlendMode)> sPipeLineStates_;

		// 汎用Vertexリソース
		ParticleModel::ModelData modelData;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;

		ParticleModel::ModelData ringModelData;
		Microsoft::WRL::ComPtr<ID3D12Resource> ringVertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW ringVertexBufferView{};

		ParticleModel::ModelData cylinderModelData;
		Microsoft::WRL::ComPtr<ID3D12Resource> cylinderVertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW cylinderVertexBufferView{};

		std::unordered_map<std::string, ParticleGroup> particleGroups;

		const uint32_t kNumMaxInstance = 100000;

		std::mt19937 randomEngine;

	private:
		std::unique_ptr<uint32_t> indexData;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;


		Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource = nullptr;
		CameraForGPU* cameraData = nullptr;
	};
}