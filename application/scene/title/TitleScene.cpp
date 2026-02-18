#include "TitleScene.h"

#include <chrono>
#include <cmath>

#include "GpuParticle.h"
#include "title/RotateCameraState.h"
#include "title/AlignCameraState.h"

#include "YamlLoader.h"
#include "Vector2Yaml.h"
#include "Vector3Yaml.h"
#include "Vector4Yaml.h"

#include "title/state/FadeInState.h"

void TitleScene::Initialize(Kouro::EngineContext context)
{
	startTime = std::chrono::steady_clock::now();

	/// 基底クラスの初期化
	BaseScene::Initialize(context);

	// ライン描画の初期化
	lineDrawer_ = std::make_unique<Kouro::LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(), sceneManager_->GetSrvManager());

	// ラインモデルマネージャーの初期化
	lineModelManager_ = std::make_unique<Kouro::LineModelManager>();
	lineModelManager_->Initialize(lineDrawer_.get());

	/// 各種素材の読み込み
	// テクスチャの読み込み
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/StartButton.png");
	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/num.png");

	// ラインモデルの読み込み
	lineModelManager_->LoadLineModel("player/player.obj");
	lineModelManager_->LoadLineModel("tunnel.obj");

	// YAMLファイルの読み込み
	YAML::Node rail_config = KOURO::YamlLoader::LoadYamlFile("title/rail_config.yaml");

	// コントロールポイントの読み込み
	controlPoints_ = rail_config["control_points"].as<std::vector<Kouro::Vector3>>();

	YAML::Node sprite_config = KOURO::YamlLoader::LoadYamlFile("title/sprite_config.yaml");

	// スタートボタンの生成
	startBotton_ = std::make_unique<Kouro::Sprite>();
	startBotton_->Initialize(Kouro::SpriteCommon::GetInstance(),"Resources/StartButton.png");

	// スタートボタンの各種設定
	startBotton_->SetPosition(sprite_config["start_button"]["position"].as<Kouro::Vector2>());
	startBotton_->SetSize(sprite_config["start_button"]["size"].as<Kouro::Vector2>());
	startBotton_->SetAnchorPoint(sprite_config["start_button"]["anchor_point"].as<Kouro::Vector2>());
	startBotton_->SetTexSize(sprite_config["start_button"]["texture_size"].as<Kouro::Vector2>());


	// プレイヤーの生成
	player_ = std::make_unique<BaseCharacter>();
	player_->Initialize(lineModelManager_->FindLineModel("player/player.obj"));
	player_->SetColor({ 0.071f, 0.429f, 1.0f,1.0f });

	// トンネル用のモデル
	tonnel_ = std::make_unique<Kouro::ObjectLine>();
	tonnel_->Initialize(lineModelManager_->FindLineModel("tunnel.obj"));

	// パーティクルエミッターの生成
	mEmitter = std::make_unique<Kouro::ModelEdgeEmitter>();
	mEmitter->Initialize("normal", context);
	mEmitter->CreateLineSegment("enemy/enemy.obj");

	pointEmitter_ = std::make_unique<Kouro::PointEmitter>();
	pointEmitter_->Initialize("normal", context);
	pointEmitter_->SetEmitterProperties({ 0.0f,0.0f,0.0f }, 1, { -4.0f,-4.0f,-10.0f }, { 4.0f,4.0f,-5.0f }, 0.5f, 1.5f, 0.0f);
	pointEmitter_->GetWorldTransform()->SetParent(player_->GetWorldTransform());

	// titleCameraの生成
	titleCamera_ = std::make_unique<TitleCamera>();
	titleCamera_->SetTarget(player_->GetWorldTransform());
	titleCamera_->Initialize();
	

	// カメラマネージャーにカメラを登録
	cameraManager_->SetActiveCamera(titleCamera_.get()/*debugCamera_.get()*/);

	// plane
	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("plane_Particle", "Resources/circle.png", Kouro::ParticleManager::ParticleType::Normal);
	// ブレンドモードの設定
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	// billboardを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableBillboard = true;

	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableStretch = true;

	// 減速を有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enableDeceleration = false;
	// パルスを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enablePulse = false;

	scoreUi_ = std::make_unique<Kouro::NumUi>();
	scoreUi_->Initialize(2);

	CreateEvents();
}

void TitleScene::Finalize()
{
	BaseScene::Finalize();
}

void TitleScene::Update()
{

	float nextT;
	Kouro::Vector3 viewTarget;
	Kouro::Vector3 position;
	Kouro::Vector3 target;
	Kouro::Vector3 forward;
	float yaw;
	float pitch;
	float roll;
	static float scale;
	static Kouro::Vector2 texSize;
	std::chrono::time_point<std::chrono::steady_clock> now;
	float elapsedTime;

	// フェーズごとの処理
	switch (phase_)
	{
		
		// フェードイン
	case Phase::kFadeIn:

		// フェードインが終了したら
		if (fade_->IsFinished())
		{
			// 入力受付を開始
			Kouro::Input::GetInstance()->SetIsReception(true);

			// フェーズをメイン部に変更
			phase_ = Phase::kMain;
		}

		break;

		// メイン部
	case Phase::kMain:

		// スペースキーまたはゲームパッドのAボタンが押されたら
		if (Kouro::Input::GetInstance()->TriggerKey(DIK_SPACE) || Kouro::Input::GetInstance()->TriggerGamePadButton(Kouro::Input::GamePadButton::A))
		{
			if (!isAlignCamera_)
			{
				isAlignCamera_ = true;

				eventController_->ExecuteEvent("CameraTransitionToAlign");
			}
		}

		// カメラがAlign状態に遷移したら
		if (isAlignCamera_)
		{
			if (titleCamera_->GetStateIsFinished())
			{
				isMoveActive_ = true;

				speedFactor_ = kDeltaTime * 0.1f;

				eventController_->ExecuteEvent("CameraTransitionToFollow");
			}
		}

		if (player_->GetWorldPosition().z >= 1200.0f)
		{
			eventController_->ExecuteEvent("CameraTransitionToLookAtPlayer");

			fade_->Start(Fade::Status::WhiteFadeOut, 3.0f);

			phase_ = Phase::kFadeOut;
		}

		// 現在の経過時間（秒）
		now = std::chrono::steady_clock::now();
		elapsedTime = std::chrono::duration<float>(now - startTime).count();

		// 時間に基づいてスケールを変化させる
		scale = std::sin(elapsedTime * 2.0f) * 0.1f + 1.0f;

		texSize = { 256.0f * scale,128.0f * scale };

		startBotton_->SetSize(texSize);

		break;

		// フェードアウト
	case Phase::kFadeOut:

		// フェードアウトが終了したら
		if (fade_->IsFinished())
		{
			// シーンをゲームシーンに変更
			Kouro::SceneManager::GetInstance()->ChangeScene("GAME");

			return;
		}

		break;

		// ゲームプレイ
	case Phase::kPlay:
		break;

		// ポーズ
	case Phase::kPose:
		break;
	}

	pointEmitter_->Update();

	startBotton_->Update();	

	if (isMoveActive_) /// 移動開始フラグが有効なら
	{
		pointEmitter_->Emit(player_->GetWorldTransform()->GetWorldMatrix());


		if (progressFactor_ < 1.0f)
		{
			progressFactor_ += speedFactor_;

			speedFactor_ *= kMultiplyFactor_;
		}

		// ワールド変換情報を取得
		auto transform_ = player_->GetWorldTransform();

		// 現在位置をCatmull-Rom補間で取得
		transform_->SetTranslate(CatmullRomPosition(controlPoints_, progressFactor_));

		// 次のフレームの位置をCatmull-Rom補間で取得
		nextT = progressFactor_ + speedFactor_;

		// 目標位置
		viewTarget = CatmullRomPosition(controlPoints_, nextT);

		// 現在位置
		position = transform_->GetTranslate();

		target = viewTarget;

		// forwardベクトル（進行方向）
		forward = Normalize(target - position);

		// forwardからオイラー角を算出
		yaw = std::atan2(forward.x, forward.z);
		pitch = std::asin(-forward.y);
		roll = 0.0f; // 必要に応じて

		transform_->SetRotate({ pitch, yaw, roll });

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

	if (ImGui::Button("emit"))
	{
		pointEmitter_->Emit(Kouro::MakeIdentity4x4());
	}

	if (Kouro::Input::GetInstance()->TriggerKey(DIK_V))
	{
		mEmitter->Emit(Kouro::MakeIdentity4x4());
	}

	ImGui::End();
#endif

	

	scoreUi_->Update();

	BaseScene::Update();
}

void TitleScene::Draw()
{
	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画	

	lineDrawer_->PreDraw(cameraManager_->GetActiveCamera()->GetViewProjection());

	player_->Draw();

	tonnel_->Draw(defaultTransform_.get());

	DrawForegroundSprite();	
	/// 前景スプライト描画	

	//scoreUi_->Draw();

	startBotton_->Draw();

	fade_->Draw();

}

void TitleScene::CreateEvents()
{
	// イベントコントローラーの生成
	eventController_ = std::make_unique<EventController>();

	// カメラの状態遷移イベントの作成

	// カメラをAlign状態に遷移させるイベント
	std::unique_ptr<Event> event = std::make_unique<Event>();
	event->SetCallback([this]() { titleCamera_->RequestChangeState(TitleCamera::State::Align); });
	eventController_->AddEvent("CameraTransitionToAlign", std::move(event));

	event = std::make_unique<Event>();
	event->SetCallback([this]() { titleCamera_->RequestChangeState(TitleCamera::State::Follow); });
	eventController_->AddEvent("CameraTransitionToFollow", std::move(event));

	event = std::make_unique<Event>();
	event->SetCallback([this]() { titleCamera_->RequestChangeState(TitleCamera::State::LookAtPlayer); });
	eventController_->AddEvent("CameraTransitionToLookAtPlayer", std::move(event));
}
