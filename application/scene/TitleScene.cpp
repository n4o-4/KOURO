#include "TitleScene.h"

#include <chrono>
#include <cmath>

#include "GpuParticle.h"

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/StartButton.png");
	TextureManager::GetInstance()->LoadTexture("Resources/num.png");


	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(), sceneManager_->GetSrvManager());

	lineModelManager_ = std::make_unique<LineModelManager>();
	lineModelManager_->Initialize(lineDrawer_.get());

	lineModelManager_->LoadLineModel("player/player.obj");



	startBotton_ = std::make_unique<Sprite>();
	startBotton_->Initialize(SpriteCommon::GetInstance(),"Resources/StartButton.png");

	startBotton_->SetPosition({ 640.0f,600.0f });
	startBotton_->SetSize({ 256.0f,128.0f });
	startBotton_->SetAnchorPoint({ 0.5f,0.5f });
	startBotton_->SetTexSize({ 1542.0f,1024.0f });

	player_ = std::make_unique<BaseCharacter>();
	player_->Initialize(lineModelManager_->FindLineModel("player/player.obj"));
	

	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->SetTarget(player_->GetWorldTransform());
	titleCamera_->Initialize();

	/*debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();*/

	cameraManager_->SetActiveCamera(titleCamera_.get()/*debugCamera_.get()*/);

	startTime = std::chrono::steady_clock::now();

	// plane
	ParticleManager::GetInstance()->CreateParticleGroup("plane_Particle", "Resources/circle.png", ParticleManager::ParticleType::Normal);
	// ブレンドモードの設定
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->blendMode = ParticleManager::BlendMode::kAdd;
	// billboardを有効
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableBillboard = true;

	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableStretch = true;

	// 減速を有効
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enableDeceleration = false;
	// パルスを有効
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enablePulse = false;


	scoreUi_ = std::make_unique<NumUi>();
	scoreUi_->Initialize(2);
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

	GpuParticle::GetInstance()->Update(cameraManager_->GetActiveCamera()->GetViewProjection());

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
			if(!isMoveActive_) /// 移動開始フラグが無効なら
			{
				titleCamera_->SetIsRotate(false);

				titleCamera_->SetIsDeparture(true);

				fadeTimer_ = 0.0f;

				isMoveActive_ = true;
			}
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

	if (isMoveActive_) /// 移動開始フラグが有効なら
	{
		if (moveTimer_ < kMoveTime)
		{
			moveTimer_ += kDeltaTime;
		}
		else
		{
			moveTimer_ = 0.0f;
		}

		auto transform_ = player_->GetWorldTransform();

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
	}

	player_->Update();

	static int count = 0;

#ifdef _DEBUG
	ImGui::Begin("Count");

	if (ImGui::DragInt("count", &count))
	{
		scoreUi_->SetDestinationValue(count,1.0f);
	}

	ImGui::End();
#endif

	scoreUi_->Update();

	BaseScene::Update();
}

void TitleScene::Draw()
{
	BaseScene::Draw();

	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画	

	//lineDrawer_->PreDraw(cameraManager_->GetActiveCamera()->GetViewProjection());

	//player_->Draw();

	DrawForegroundSprite();	
	/// 前景スプライト描画	

	//scoreUi_->Draw();

	startBotton_->Draw();

	fade_->Draw();

}
