#include "BaseScene.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "ParticleManager.h"

namespace Kouro
{
	void BaseScene::Initialize(EngineContext context)
	{
		// ライン描画クラスの生成と初期化
		lineDrawer_ = std::make_unique<LineDrawerBase>();
		lineDrawer_->Initialize(sceneManager_->GetDxCommon(), srvManager_);

		// カメラマネージャークラスの生成と初期化
		cameraManager_ = std::make_unique<CameraManager>();
		cameraManager_->Initialize();

		// フェードクラスの生成と初期化
		fade_ = std::make_unique<Fade>();
		fade_->Initialize();

		// フェーズをフェードインに設定
		phase_ = Phase::kFadeIn;

		// フェードを設定してスタート
		fade_->Start(Fade::Status::FadeIn, fadeTime_);

		defaultTransform_ = std::make_unique<WorldTransform>();
		defaultTransform_->Initialize();

		// 入力の受付を無効に
		Input::GetInstance()->SetIsReception(false);

		// ポストエフェクトクラスにカメラマネージャークラスのポインタを渡す
		sceneManager_->GetPostEffect()->SetCameraManager(cameraManager_.get());

		// パーティクルマネージャークラスの生成と初期化
		ParticleManager::GetInstance()->Initialize(sceneManager_->GetDxCommon(), srvManager_);

		// パーティクルマネージャクラスにカメラマネージャークラスのポインタを渡す
		ParticleManager::GetInstance()->SetCameraManager(cameraManager_.get());

		// スカイボックスの生成と初期化
		skybox_ = std::make_unique<Skybox>();
		skybox_->Initialize(sceneManager_->GetDxCommon(), "Resources/envMap.dds");

		// オブジェクト3dコモンに環境マップテクスチャを設定
		Object3dCommon::GetInstance()->SetEnvironmentMapPath("Resources/envMap.dds");

		engineContext_ = context;
	}

	void BaseScene::Finalize()
	{
		AudioManager::GetInstance()->Finalize();

		ParticleManager::GetInstance()->Finalize();
	}

	void BaseScene::Update()
	{
		// カメラマネージャーの更新
		cameraManager_->Update();

		// フェードの更新
		fade_->Update();
	}

	void BaseScene::LineDraw()
	{
		// 線の描画前処理
		lineDrawer_->PreDraw(Camera::GetInstance()->GetViewProjection());
	}

	void BaseScene::DrawObject()
	{
		// オブジェクト3dの描画前処理
		Object3dCommon::GetInstance()->SetView();

	}

	void BaseScene::DrawBackgroundSprite()
	{
		// 背景用スプライトの描画前処理
		SpriteCommon::GetInstance()->SetBackgroundView();

		SpriteCommon::GetInstance()->DrawBackground();
	}

	void BaseScene::DrawForegroundSprite()
	{
		// 前景描画用スプライトの描画前処理
		SpriteCommon::GetInstance()->SetForegroundView();

		SpriteCommon::GetInstance()->DrawForeground();
	}

	void BaseScene::DrawFade()
	{
		// フェードスプライトの描画
		fade_->Draw();
	}
}