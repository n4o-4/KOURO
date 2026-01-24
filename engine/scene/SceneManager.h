#pragma once
#include <memory>
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "Camera.h"
#include "SrvManager.h"
#include "PostEffect.h"
#include "EngineContext.h"

// \brief SceneManager
// ゲーム内のシーン管理を行うシングルトンクラス。
// 現在のシーンや次のシーンを保持し、更新・描画処理やシーン切り替えを管理する。
// SceneFactoryを利用してシーンを生成し、PostEffectやカメラなどの共通リソースも保持。

namespace Kouro
{
	class SceneManager
	{
	public:

		static SceneManager* GetInstance();

		/**
		* \brief  Initialize 初期化
		* \param  DirectXCommon DirectXCommonのポインタ
		* \param  SrvManager Srvマネージャーのポインタ
		* \param  Camera カメラのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager, Camera* camera, EngineContext context);

		// \brief Finalize 終了
		void Finalize();

		// \brief Update 更新
		void Update();

		// \brief Draw 描画	
		void Draw();

		/**
		* \brief  ChangeState シーンを変える
		* \param  sceneName シーンの名前
		*/
		void ChangeScene(const std::string& sceneName);

		/**
		* \brief  SetSceneFactory sceneFactoryの設定
		* \param  sceneFactory
		*/
		void SetSceneFactory(AbstractSceneFactory& sceneFactory) { sceneFactory_ = &sceneFactory; }

		/**
		* \brief  SetPostEffect ポストエフェクトクラスの設定
		* \param  PostEffect posEffectのポインタ
		*/
		void SetPostEffect(PostEffect* postEffect) { postEffect_ = postEffect; }

		/**
		* \brief  GetDxCommon DirectXCommonのポインタを取得する
		* \return dxCommon_ DirectXCommonのポインタ
		*/
		DirectXCommon* GetDxCommon() { return dxCommon_; }

		/**
		* \brief  GetSrvManager SrvManagerのポインタを取得す
		* \return srvManager_ SrvManagerのポインタ
		*/
		SrvManager* GetSrvManager() { return srvManager_; }

		/**
		* \brief  GetCamera Cameraのポインタを取得する
		* \return camera_ Cameraのポインタ
		*/
		Camera* GetCamera() { return camera_; }

		/**
		* \brief  GetPostEffect PostEffectのポインタを取得する
		* \return postEffect_ PostEffectのポインタ
		*/
		PostEffect* GetPostEffect() { return postEffect_; }

		const ViewProjection GetViewProjection() const { return scene_->GetViewProjection(); }

	private:

		static std::unique_ptr<SceneManager> instance;

		friend std::unique_ptr<SceneManager> std::make_unique<SceneManager>();
		friend std::default_delete<SceneManager>;

		~SceneManager() = default;
		SceneManager() = default;
		SceneManager(SceneManager&) = delete;
		SceneManager& operator=(SceneManager&) = delete;


		// 今のシーン
		std::unique_ptr<BaseScene> scene_ = nullptr;

		// 次のシーン
		std::unique_ptr<BaseScene> nextScene_ = nullptr;

		AbstractSceneFactory* sceneFactory_ = nullptr;

		SrvManager* srvManager_ = nullptr;

		DirectXCommon* dxCommon_ = nullptr;

		Camera* camera_ = nullptr;

		PostEffect* postEffect_ = nullptr;

		EngineContext engineContext_;
	};
}