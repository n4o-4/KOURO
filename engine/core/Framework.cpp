#include "Framework.h"

#include "LeakChecker.h"

namespace Kouro
{
	void Framework::Initialize()
	{
		// ゲーム開始時間を記録
		startTime_ = std::chrono::steady_clock::now();

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

		SceneManager::GetInstance()->Initialize(dxCommon_, srvManager_.get(), Camera::GetInstance(), CreateEngineContext());

		lineDrawer_ = std::make_unique<LineDrawerBase>();
		lineDrawer_->Initialize(dxCommon_, srvManager_.get());

		postEffect_ = std::make_unique<PostEffect>();
		postEffect_->Initialize(dxCommon_, srvManager_.get());

		SceneManager::GetInstance()->SetPostEffect(postEffect_.get());

		gpuParticle_ = GpuParticle::GetInstance();
		//gpuParticle_->Initialize(dxCommon_, srvManager.get(), uavManager_.get());
	}

	void Framework::Finalize()
	{
		GpuParticle::GetInstance()->Finalize();

		SceneManager::GetInstance()->Finalize();

		postEffect_->Finalize();

		ParticleManager::GetInstance()->Finalize();

		Camera::GetInstance()->Finalize();

		ModelManager::GetInstance()->Finalize();

		Object3dCommon::GetInstance()->Finalize();

		SpriteCommon::GetInstance()->Finalize();

		TextureManager::GetInstance()->Finalize();

		Input::GetInstance()->Finalize();

		DirectXCommon::GetInstance()->Finalize();

		winApp->Finalize();
	}

	void Framework::Update()
	{
		Camera::GetInstance()->Update();

		// メッセージ処理
		if (winApp->ProcessMessage()) {
			endRequest_ = true;
		}

		// 入力情報の更新
		Input::GetInstance()->Update();

		// シーンマネージャーの更新
		SceneManager::GetInstance()->Update();

		UpdateFPS();

		gpuParticleManager_->SetPerFrame(elapsedTime_.count(), deltaTime_.count());
	}

	void Framework::Draw()
	{
		auto sceneManager = SceneManager::GetInstance();

		sceneManager->Draw();
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
		// GpuContextの作成
		GpuContext gpuContext;

		gpuContext.d3d12Context = DirectXCommon::GetInstance()->CreateD3D12Context();
		gpuContext.srvManager = srvManager_.get();
		gpuContext.uavManager = uavManager_.get();
		gpuContext.gpuResourceUtils = gpuResourceUtils_.get();

		return gpuContext;
	}

	EngineContext Framework::CreateEngineContext()
	{
		// EngineContextの作成
		EngineContext engineContext;

		engineContext.gpuContext = CreateGpuContext();
		engineContext.gpuParticleManager = gpuParticleManager_.get();

		return engineContext;
	}

	void Framework::UpdateFPS()
	{
		// 現在の時間を取得
		now_ = std::chrono::steady_clock::now();

		// 前回のフレームからの経過時間を計算
		deltaTime_ = std::chrono::duration<float>(now_ - prevTime_);

		// 経過時間をキューに追加
		deltaTimes_.push_back(deltaTime_);

		// 過去1秒より古いフレームを削除
		float accumulated = 0.0f;
		for (auto it = deltaTimes_.rbegin(); it != deltaTimes_.rend(); ++it)
		{
			// 経過時間を累積
			accumulated += it->count();

			// 累積時間が1秒を超えたら、それ以降の古いフレームを削除
			if (accumulated > frameHistoryDuration_)
			{
				// これ以降の古いフレームを削除
				deltaTimes_.erase(deltaTimes_.begin(), it.base());

				break;
			}
		}

		// フレームカウントを更新
		currentFrameCount_++;

		// 前回のフレームの時間を更新
		prevTime_ = now_;

		elapsedTime_ = now_ - startTime_;

#ifdef _DEBUG

		// フレーム
		ImGui::Begin("FrameData");
		
		// 経過時間用の枠
		ImGui::BeginChild("ElapsedTimeBox", ImVec2(192, 32), true);
		ImGui::Text("Elapsed Time: %.2f s", elapsedTime_.count());
		ImGui::EndChild();

		// 累計フレーム用の枠
		ImGui::BeginChild("FrameCountBox", ImVec2(192, 32), true);
		ImGui::Text("Frame Count : %u", currentFrameCount_);
		ImGui::EndChild();

		// FPS用の枠
		ImGui::BeginChild("FPSBox", ImVec2(192, 32), true);
		float fps = (accumulated > 0.0f) ? deltaTimes_.size() / accumulated : 0.0f;
		ImGui::Text("FPS         : %.1f", fps);
		ImGui::EndChild();


		ImGui::End();
#endif
	}
}