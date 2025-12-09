#include "Framework.h"

#include "LeakChecker.h"

void Framework::Initialize()
{
	// WinAppの生成と初期化
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	// DirectXCommonの生成と初期化
	DirectXCommon::GetInstance()->Initialize(winApp.get());
	dxCommon_ = DirectXCommon::GetInstance();

	// Inputの生成と初期化
	Input::GetInstance()->Initialize(winApp.get());

	// SrvManagerの生成と初期化
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Initialize(dxCommon_);

	// UavManagerの生成と初期化
	uavManager_ = std::make_unique<UavManager>();
	uavManager_->Initialize(dxCommon_);

	gpuResourceUtils_ = std::make_unique<GpuResourceUtils>(dxCommon_->GetDevice().Get());

	// TextureManagerの初期化
	TextureManager::GetInstance()->Initialize(dxCommon_, srvManager_.get());

	// SpriteCommon初期化
	SpriteCommon::GetInstance()->Initialize(dxCommon_);

	// Object3dCommonの生成と初期化
	Object3dCommon::GetInstance()->Initialize(dxCommon_);

	// ModelCommonの生成と初期化
	modelCommon = std::make_unique<ModelCommon>();
	modelCommon->Initialize(dxCommon_);

	// ModelManagerの生成と初期化
	ModelManager::GetInstance()->Initialize(dxCommon_);

	// Cameraの生成と初期化
	Camera::GetInstance()->Initialize();

	// ParticleManagerの生成と初期化
	ParticleManager::GetInstance()->Initialize(dxCommon_, srvManager_.get());
	
	gpuParticleManager_ = std::make_unique<GpuParticleManager>(CreateGpuContext());
	gpuParticleManager_->Initialize();

	SceneManager::GetInstance()->Initialize(dxCommon_,srvManager_.get(),Camera::GetInstance(),CreateEngineContext());

	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(dxCommon_,srvManager_.get());

	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(dxCommon_,srvManager_.get());

	SceneManager::GetInstance()->SetPostEffect(postEffect_.get());

	gpuParticle_ = GpuParticle::GetInstance();
	//gpuParticle_->Initialize(dxCommon_, srvManager.get(), uavManager_.get());

	now = std::chrono::steady_clock::now();

	startTime = now;
}

void Framework::Finalize()
{
	GpuParticle::GetInstance()->Finalize();

	SceneManager::GetInstance()->Finalize(); ///

	postEffect_->Finalize();

	ParticleManager::GetInstance()->Finalize(); ///

	Camera::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize(); ///

	Object3dCommon::GetInstance()->Finalize(); ///

	SpriteCommon::GetInstance()->Finalize(); ///

	TextureManager::GetInstance()->Finalize(); ///

	Input::GetInstance()->Finalize(); //

	DirectXCommon::GetInstance()->Finalize();

	winApp->Finalize();

}

void Framework::Update()
{

	Camera::GetInstance()->Update();

	if (winApp->ProcessMessage()) {
		endRequest_ = true;
	}

	Input::GetInstance()->Update();

	SceneManager::GetInstance()->Update();

	gpuParticleManager_->Update();

#ifdef _DEBUG

	UpdateFPS();

	gpuParticleManager_->SetPerFrame(static_cast<float >(totalTime), deltaTime);

#endif _DEBUG
}

void Framework::Draw()
{

	SceneManager::GetInstance()->Draw();

	//gpuParticleManager_->
}

void Framework::DrawEffect()
{
	postEffect_->Draw();
}



void Framework::Run()
{

	// ゲームの初期化
	Initialize();

	while (true)
	{

		// 毎フレーム更新
		Update();

		if (IsEndRequest())
		{
			break;
		}

		Draw();
	}

	// ゲームの終了
	Finalize();
}

GpuContext Framework::CreateGpuContext()
{
	GpuContext gpuContext;

	gpuContext.d3d12Context = DirectXCommon::GetInstance()->CreateD3D12Context();
	gpuContext.srvManager = srvManager_.get();
	gpuContext.uavManager = uavManager_.get();
	gpuContext.gpuResourceUtils = gpuResourceUtils_.get();

	return gpuContext;
}

EngineContext Framework::CreateEngineContext()
{
	EngineContext engineContext;
	
	engineContext.gpuContext = CreateGpuContext();
	engineContext.gpuParticleManager = gpuParticleManager_.get();

	return engineContext;
}

void Framework::UpdateFPS()
{
	frameCount++;

	// 現在時刻を取得
	now = std::chrono::steady_clock::now();

	// 経過時間を秒で計算
	//elapsedTime = std::chrono::duration<float>(now - lastTime).count();
	totalTime = std::chrono::duration<float>(now - startTime).count();

	if (elapsedTime >= 1.0f) {
		fps = frameCount / elapsedTime;
		frameCount = 0;
	}

	deltaTime = std::chrono::duration<float>(now - lastTime).count();

	lastTime = now; // lastTime をここで更新

	// ImGui に表示
	ImGui::Text("FPS: %.2f TotalTime: %.2f deltaTime: %.f", fps, totalTime,deltaTime);
}
