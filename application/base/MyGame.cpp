#include "MyGame.h"

#include "SceneFactory.h"


void MyGame::Initialize()
{
#pragma region 基盤システムの初期化

	// 基盤システムの初期化
	Framework::Initialize();

#ifdef _DEBUG
	
	// ImGuiマネージャーの生成と初期化
	imGuiManager = std::make_unique<Kouro::ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), Kouro::DirectXCommon::GetInstance());

#endif

	// シーンファクトリーの生成とシーンマネージャーへの登録
	sceneFactory_ = std::make_unique<SceneFactory>();
	Kouro::SceneManager::GetInstance()->SetSceneFactory(*sceneFactory_);

	// 初期シーンの設定
	Kouro::SceneManager::GetInstance()->ChangeScene("TITLE");

#pragma endregion
}

void MyGame::Finalize()
{
	// 基盤システムの終了
	Framework::Finalize();
}

void MyGame::Update()
{
#ifdef _DEBUG

	//  ImGuiの開始
	imGuiManager->Begin();

#endif

	// 基盤システムの更新
	Framework::Update();

	gpuParticleManager_->Update();

	// ポストエフェクトの更新
	postEffect_->Update();
}

void MyGame::Draw()
{
	// レンダーテクスチャへの描画前処理
	Kouro::DirectXCommon::GetInstance()->RenderTexturePreDraw();

	// SRVマネージャーの描画前処理
    srvManager_->PreDraw();

	// 基盤システムの描画
	Framework::Draw();

	gpuParticleManager_->Draw(Kouro::SceneManager::GetInstance()->GetViewProjection());

	// レンダーテクスチャへの描画後処理
	Kouro::DirectXCommon::GetInstance()->RenderTexturePostDraw();

	// エフェクトの描画
	Framework::DrawEffect();

	// 画面への描画前処理
	Kouro::DirectXCommon::GetInstance()->PreDraw();

#ifdef _DEBUG

	// ImGuiの描画
	imGuiManager->Draw(Kouro::DirectXCommon::GetInstance());

#endif

	// 基盤システムの描画後処理
	Kouro::DirectXCommon::GetInstance()->PostDraw();
}
