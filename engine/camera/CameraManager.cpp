#include "CameraManager.h"

namespace Kouro
{
	void CameraManager::Initialize()
	{
		cameraRegistry_["DebugCamera"] = std::make_unique<DebugCamera>();
		cameraRegistry_["DebugCamera"]->Initialize();

		// シード値の初期化
		std::random_device seedGenerator;
		randomEngine.seed(seedGenerator());
	}

	void CameraManager::Update()
	{
		// カメラシェイク処理
		Shake();

		// シェイクオフセットをアクティブカメラに適用
		activeCamera_->SetShakeOffset(shakeOffSet);

		// アクティブカメラの更新
		activeCamera_->Update();

		DrawDebugUI();
	}

	void CameraManager::SetActiveCamera(std::string name)
	{
		// 名前でカメラを検索し、アクティブカメラを設定
		auto it = cameraRegistry_.find(name);
		if (it != cameraRegistry_.end())
		{
			activeCamera_ = it->second.get();
		}
		else
		{
			assert(1); // カメラが見つからない場合はアサート
		}
	}

	void CameraManager::CamerasClear()
	{
		cameraRegistry_.clear();
	}

	void CameraManager::CameraShake(float time)
	{
		shakeTime = time;

		isShaking = true;
	}

	void CameraManager::Shake()
	{
		// シェイク時間の管理
		timer += 1.0f / 60.0f;

		// シェイク時間終了後、シェイクを停止
		if (shakeTime < timer)
		{
			isShaking = false;
			timer = 0.0f;
			shakeTime = 0.0f;

			shakeOffSet = { 0.0f,0.0f,0.0f };
		}

		// シェイク中でなければ抜ける
		if (!isShaking) return;

		// カメラの現在位置を取得
		Vector3 translate = activeCamera_->GetWorldTransform().GetTranslate();

		// ランダムなオフセットを生成
		std::uniform_real_distribution<float> dist(-kShakeAmplitude, kShakeAmplitude);

		// シェイクオフセットを設定
		shakeOffSet = { dist(randomEngine),dist(randomEngine),dist(randomEngine) };
	}

	void CameraManager::DrawDebugUI()
	{
#ifdef _DEBUG

		ImGui::Begin("Camera Manager");

		for (auto& [name, camera] : cameraRegistry_)
		{
			bool isActive = (camera.get() == activeCamera_);

			if (ImGui::RadioButton(name.c_str(), isActive))
			{
				activeCamera_ = camera.get();
			}
		}

		ImGui::End();

#endif
	}
}