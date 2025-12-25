#include "CameraManager.h"

void CameraManager::Initialize()
{
	// デバッグカメラの初期化
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	// 追従カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 最初のアクティブカメラは通常カメラに設定
	activeCamera_ = Camera::GetInstance();

	// カメラリストに各カメラを追加
	cameras_.push_back(Camera::GetInstance());
	cameras_.push_back(debugCamera_.get());

	// シード値の初期化
	std::random_device seedGenerator;
	randomEngine.seed(seedGenerator());
}

void CameraManager::Update()
{
	// カメラシェイク処理
	Shake();

	// シェイクオフセットをアクティブカメラに適用
	activeCamera_->SetOffset(shakeOffSet);

	// アクティブカメラの更新
	activeCamera_->Update();

	DrawDebugUI();
}

void CameraManager::CamerasClear()
{
	cameras_.clear();
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
	Vector3 translate = activeCamera_->GetViewProjection().transform.translate;

	// ランダムなオフセットを生成
	std::uniform_real_distribution<float> dist(-0.2f, 0.2f);

	// シェイクオフセットを設定
	shakeOffSet = { dist(randomEngine),dist(randomEngine),dist(randomEngine) };
}

void CameraManager::DrawDebugUI()
{
#ifdef _DEBUG

	ImGui::Begin("CameraManager");

	if(ImGui::Button("Use Debug Camera"))
	{
		activeCamera_ = debugCamera_.get();
	}

	if (ImGui::Button("Use FollowCamera"))
	{
		activeCamera_ = followCamera_.get();
	}

	ImGui::End();

#endif
}
