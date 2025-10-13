#include "TitleScene.h"

#include <chrono>
#include <cmath>

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/StartButton.png");

	startBotton_ = std::make_unique<Sprite>();
	startBotton_->Initialize(SpriteCommon::GetInstance(),"Resources/StartButton.png");

	startBotton_->SetPosition({ 640.0f,600.0f });
	startBotton_->SetSize({ 256.0f,128.0f });
	startBotton_->SetAnchorPoint({ 0.5f,0.5f });
	startBotton_->SetTexSize({ 1542.0f,1024.0f });

	transform_ = std::make_unique<WorldTransform>();

	transform_->Initialize();

	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(), sceneManager_->GetSrvManager());

	lineDrawer_->CreateObject3DLine("player/player.obj", transform_.get());

	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->SetTarget(transform_.get());
	titleCamera_->Initialize();

	cameraManager_->SetActiveCamera(titleCamera_.get());

	startTime = std::chrono::steady_clock::now();
}

void TitleScene::Finalize()
{
	BaseScene::Finalize();
}

void TitleScene::Update()
{
	

	constexpr float kMoveTime = 5.0f;

	static float moveTimer_ = 0.0f; //!< カメラの移動時間

	float t = moveTimer_ / kMoveTime;

	float nextT;
	Vector3 viewTarget;
	Vector3 position;
	Vector3 target;
	Vector3 forward;
	float yaw;
	float pitch;
	float roll;
	static float scale;
	static Vector2 texSize;
	std::chrono::time_point<std::chrono::steady_clock> now;
	float elapsedTime;

	switch (phase_)
	{
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{
			Input::GetInstance()->SetIsReception(true);

			titleCamera_->SetIsRotate(true);

			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if (Input::GetInstance()->Triggerkey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A))
		{
			titleCamera_->SetIsRotate(false);

			titleCamera_->SetIsDeparture(true);

			fadeTimer_ = 0.0f;
		}

		if (titleCamera_->GetIsDeparture())
		{
			fadeTimer_ += kDeltaTime;

			if (fadeTimer_ >= kFadeStartTime)
			{
				fade_->Start(Fade::Status::WhiteFadeOut, 0.5f);
				phase_ = Phase::kFadeOut;
			}
		}

		// 現在の経過時間（秒）
		now = std::chrono::steady_clock::now();
		elapsedTime = std::chrono::duration<float>(now - startTime).count();

		// 時間に基づいてスケールを変化させる
		scale = std::sin(elapsedTime * 2.0f) * 0.1f + 1.0f;

		texSize = { 256.0f * scale,128.0f * scale };

		startBotton_->SetSize(texSize);

		break;
	case Phase::kFadeOut:

		if (moveTimer_ < kMoveTime)
		{
			moveTimer_ += kDeltaTime;
		}
		else
		{
			moveTimer_ = 0.0f;
		}

		transform_->transform.translate = CatmullRomPosition(controlPoints_, t);

		nextT = (moveTimer_ + (kDeltaTime * 26.0f)) / kMoveTime;

		viewTarget = CatmullRomPosition(controlPoints_, nextT);

		// 現在位置
		position = transform_->transform.translate;

		target = viewTarget;

		// forwardベクトル（進行方向）
		forward = Normalize(target - position);

		// forwardからオイラー角を算出
		yaw = std::atan2(forward.x, forward.z);
		pitch = std::asin(-forward.y);
		roll = 0.0f; // 必要に応じて

		transform_->transform.rotate = { pitch, yaw, roll };

		transform_->UpdateMatrix();

		if (fade_->IsFinished())
		{
			SceneManager::GetInstance()->ChangeScene("GAME");

			return;
		}

		break;

	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	startBotton_->Update();

	transform_->UpdateMatrix();

	lineDrawer_->Update();

	BaseScene::Update();
}

void TitleScene::Draw()
{
	BaseScene::Draw();

	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画	

	lineDrawer_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection());

	DrawForegroundSprite();	
	/// 前景スプライト描画	

	startBotton_->Draw();

	fade_->Draw();

}
