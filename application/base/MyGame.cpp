#include "MyGame.h"

#include "SceneFactory.h"


void MyGame::Initialize()
{

#pragma region 基盤システムの初期化

	
	Framework::Initialize();

#ifdef _DEBUG
	
	imGuiManager = std::make_unique<ImGuiManager>();
	imGuiManager->Initialize(winApp.get(), DirectXCommon::GetInstance());

#endif

	sceneFactory_ = std::make_unique<SceneFactory>();

	SceneManager::GetInstance()->SetSceneFactory(*sceneFactory_);

	SceneManager::GetInstance()->ChangeScene("GAME");

#pragma endregion 基盤システムの初期化

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");
}

void MyGame::Finalize()
{

	Framework::Finalize();

}

void MyGame::Update()
{
#ifdef _DEBUG

	imGuiManager->Begin();

#endif

	Framework::Update();

	postEffect_->Update();

#ifdef _DEBUG

	imGuiManager->End();

#endif

}

void MyGame::Draw()
{
	DirectXCommon::GetInstance()->RenderTexturePreDraw();

    srvManager->PreDraw();

	

	Framework::Draw();

	DirectXCommon::GetInstance()->RenderTexturePostDraw();

	Framework::DrawEffect();

	DirectXCommon::GetInstance()->PreDraw();


#ifdef _DEBUG

	imGuiManager->Draw(DirectXCommon::GetInstance());

	//========================================
	// LineのImGui描画
	//LineManager::GetInstance()->DrawImGui();

#endif

	DirectXCommon::GetInstance()->PostDraw();

}
