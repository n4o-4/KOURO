#include "Framework.h"

#include "LeakChecker.h"
#include "ShaderCompiler.h"

namespace Kouro
{
	void Framework::Initialize()
	{
		// ゲーム開始時間を記録
		startTime_ = std::chrono::steady_clock::now();

		// WinAppの生成と初期化
		winApp_ = std::make_unique<WinApp>();
		winApp_->Initialize();

		// DirectXCommonの生成と初期化
		DirectXCommon::GetInstance()->Initialize(winApp_.get());
		dxCommon_ = DirectXCommon::GetInstance();

		gpuResourceUtils_ = &dxCommon_->GetGpuResourceUtils();

		D3D12Context d3d12Context = dxCommon_->CreateD3D12Context();

		CompileShaders();

		// Inputの生成と初期化
		Input::GetInstance()->Initialize(winApp_.get());

		// SrvManagerの生成と初期化
		srvManager_ = std::make_unique<SrvManager>();
		srvManager_->Initialize(dxCommon_);

		// UavManagerの生成と初期化
		uavManager_ = std::make_unique<UavManager>();
		uavManager_->Initialize(dxCommon_);

		// TextureManagerの初期化
		TextureManager::GetInstance()->Initialize(dxCommon_, srvManager_.get());

		// SpriteCommon初期化
		SpriteCommon::GetInstance()->Initialize(d3d12Context.device,d3d12Context.commandList, shaderManager_.GetShader("Sprite"));

		// Object3dCommonの生成と初期化
		Object3dCommon::GetInstance()->Initialize(dxCommon_);

		// ModelCommonの生成と初期化
		modelCommon_ = std::make_unique<ModelCommon>();
		modelCommon_->Initialize(dxCommon_);

		// ModelManagerの生成と初期化
		ModelManager::GetInstance()->Initialize(dxCommon_);

		// Cameraの生成と初期化
		Camera::GetInstance()->Initialize();

		// ParticleManagerの生成と初期化
		ParticleManager::GetInstance()->Initialize(dxCommon_, srvManager_.get());

		gpuParticleManager_ = std::make_unique<GpuParticleManager>(CreateGpuContext());
		gpuParticleManager_->Initialize();

		SceneManager::GetInstance()->Initialize(dxCommon_, srvManager_.get(), Camera::GetInstance(), CreateEngineContext());

		postEffect_ = std::make_unique<PostEffect>();
		postEffect_->Initialize(dxCommon_, srvManager_.get());

		SceneManager::GetInstance()->SetPostEffect(postEffect_.get());

		gpuParticle_ = GpuParticle::GetInstance();
		//gpuParticle_->Initialize(dxCommon_, srvManager.get(), uavManager_.get());

#ifdef _DEBUG

        // ImGuiマネージャーの生成と初期化
		imGuiManager_ = std::make_unique<Kouro::ImGuiManager>();
		imGuiManager_->Initialize(winApp_.get(), Kouro::DirectXCommon::GetInstance());

#endif
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

		winApp_->Finalize();
	}

	void Framework::Update()
	{
		Camera::GetInstance()->Update();

		// メッセージ処理
		if (winApp_->ProcessMessage()) {
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

		gpuContext.d3d12Context = dxCommon_->CreateD3D12Context();
		gpuContext.srvManager = srvManager_.get();
		gpuContext.uavManager = uavManager_.get();
		gpuContext.gpuResourceUtils = gpuResourceUtils_;

		return gpuContext;
	}

	EngineContext Framework::CreateEngineContext()
	{
		// EngineContextの作成
		EngineContext engineContext;

		engineContext.gpuContext = CreateGpuContext();
		engineContext.gpuParticleManager = gpuParticleManager_.get();
		engineContext.shaderManager = shaderManager_;

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

	void Framework::CompileShaders()
	{
		// コンパイラの初期化
		ShaderCompiler compiler;
		compiler.Initialize();
		Shader::GraphicsShader compileResult;

		// シェーダープロファイルの取得
		std::wstring vsProfile = compiler.GetPixelShaderProfile(ShaderCompiler::ShaderType::VertexShader);
		std::wstring psProfile = compiler.GetPixelShaderProfile(ShaderCompiler::ShaderType::PixelShader);

		// Sprite用シェーダーのコンパイルと追加
		compileResult.vertexShader = compiler.CompileShader(L"Resources/Shaders/SpriteVS.hlsl", vsProfile.c_str());
		compileResult.pixelShader = compiler.CompileShader(L"Resources/Shaders/SpritePS.hlsl", psProfile.c_str());
		shaderManager_.AddShader("Sprite", compileResult);

		compileResult.vertexShader = compiler.CompileShader(L"Resources/shaders/PrimitiveDrawerVS.hlsl", vsProfile.c_str());
		compileResult.pixelShader = compiler.CompileShader(L"Resources/shaders/PrimitiveDrawerPS.hlsl", psProfile.c_str());
		shaderManager_.AddShader("PrimitiveDrawer", compileResult);
	}
}