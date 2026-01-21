#include "TitleScene.h"

#include <chrono>
#include <cmath>

#include "GpuParticle.h"
#include "RotateCameraState.h"
#include "FollowCameraState.h"

#include "YamlLoader.h"
#include "Vector3Yaml.h"

void TitleScene::Initialize(EngineContext context)
{
	/// 基底クラスの初期化
	BaseScene::Initialize(context);

	// ライン描画の初期化
	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(), sceneManager_->GetSrvManager());

	// ラインモデルマネージャーの初期化
	lineModelManager_ = std::make_unique<LineModelManager>();
	lineModelManager_->Initialize(lineDrawer_.get());

	/// 各種素材の読み込み
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/StartButton.png");
	TextureManager::GetInstance()->LoadTexture("Resources/num.png");

	// ラインモデルの読み込み
	lineModelManager_->LoadLineModel("player/player.obj");
	lineModelManager_->LoadLineModel("tunnel.obj");

	// YAMLファイルの読み込み
	YAML::Node rail_config = KOURO::YamlLoader::LoadYamlFile("title/rail_config.yaml");

	// コントロールポイントの読み込み
	controlPoints_ = rail_config["control_points"].as<std::vector<Vector3>>();

	YAML::Node sprite_config = KOURO::YamlLoader::LoadYamlFile("title/sprite_config.yaml");

	// スタートボタンの生成
	startBotton_ = std::make_unique<Sprite>();
	startBotton_->Initialize(SpriteCommon::GetInstance(),"Resources/StartButton.png");

	// スタートボタンの各種設定
	startBotton_->SetPosition({ 640.0f,600.0f });
	startBotton_->SetSize({ 256.0f,128.0f });
	startBotton_->SetAnchorPoint({ 0.5f,0.5f });
	startBotton_->SetTexSize({ 1542.0f,1024.0f });

	// プレイヤーの生成
	player_ = std::make_unique<BaseCharacter>();
	player_->Initialize(lineModelManager_->FindLineModel("player/player.obj"));
	player_->SetColor({ 0.071f, 0.429f, 1.0f,1.0f });

	// トンネル用のモデル
	tonnel_ = std::make_unique<ObjectLine>();
	tonnel_->Initialize(lineModelManager_->FindLineModel("tunnel.obj"));

	// パーティクルエミッターの生成
	mEmitter = std::make_unique<ModelEdgeEmitter>();
	mEmitter->Initialize("normal", context);
	mEmitter->CreateLineSegment("enemy/enemy.obj");

	// titleCameraの生成
	titleCamera_ = std::make_unique<TitleCamera>();
	//titleCamera_->SetTarget(player_->GetWorldTransform());
	titleCamera_->Initialize();
	std::unique_ptr<RotateCameraState> newState = std::make_unique<RotateCameraState>(titleCamera_.get(), player_->GetWorldTransform());
	titleCamera_->ChangeState(std::move(newState));

	/*debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();*/

	// カメラマネージャーにカメラを登録
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

	// フェーズごとの処理
	switch (phase_)
	{
		
		// フェードイン
	case Phase::kFadeIn:

		// フェードインが終了したら
		if (fade_->IsFinished())
		{
			// 入力受付を開始
			Input::GetInstance()->SetIsReception(true);

			// カメラを回転させる
			titleCamera_->SetIsRotate(true);

			// フェーズをメイン部に変更
			phase_ = Phase::kMain;
		}

		break;

		// メイン部
	case Phase::kMain:

		// スペースキーまたはゲームパッドのAボタンが押されたら
		if (Input::GetInstance()->Triggerkey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A))
		{
			// 移動開始フラグが無効なら
			if(!isMoveActive_)
			{
				cameraManager_->GetActiveCamera()->ChangeState(std::make_unique<FollowCameraState>(cameraManager_->GetActiveCamera(), player_->GetWorldTransform()));

				// カメラの回転を停止
				titleCamera_->SetIsRotate(false);

				// 離脱フラグを有効にする
				titleCamera_->SetIsDeparture(true);

				// フェードタイマーをリセット
				fadeTimer_ = 0.0f;

				// 移動開始フラグを有効にする
				isMoveActive_ = true;

				speedFactor_ = kDeltaTime * 0.1f;
			}
		}

		// 離脱フラグが有効なら
		if (titleCamera_->GetIsDeparture())
		{
			// フェードタイマーを加算
			fadeTimer_ += kDeltaTime;

			// フェード開始時間を超えたら
			if (fadeTimer_ >= kFadeStartTime)
			{
				// 白フェードアウトを開始
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

		// フェードアウト
	case Phase::kFadeOut:

		// フェードアウトが終了したら
		if (fade_->IsFinished())
		{
			// シーンをゲームシーンに変更
			SceneManager::GetInstance()->ChangeScene("GAME");

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

	startBotton_->Update();

	if (isMoveActive_) /// 移動開始フラグが有効なら
	{
		if (progressFactor_ < 1.0f)
		{
			progressFactor_ += speedFactor_;

			speedFactor_ *= kMultiplyFactor_;
		}

		// ワールド変換情報を取得
		auto transform_ = player_->GetWorldTransform();

		// 現在位置をCatmull-Rom補間で取得
		transform_->transform.translate = CatmullRomPosition(controlPoints_, progressFactor_);

		// 次のフレームの位置をCatmull-Rom補間で取得
		nextT = progressFactor_ + speedFactor_;

		// 目標位置
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

	if (ImGui::Button("emit"))
	{
		mEmitter->Emit(MakeIdentity4x4());
	}

	ImGui::End();
#endif

	if (Input::GetInstance()->Triggerkey(DIK_V))
	{
		mEmitter->Emit(MakeIdentity4x4());
	}

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
