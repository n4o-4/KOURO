#include "GameScene.h"
#include "imgui.h"
#include <imgui_internal.h>

#include "Easing.h"
#include "SceneCommand.h"
#include "YamlLoader.h"
#include "Vector2Yaml.h"
#include "Vector3Yaml.h"
#include "Vector4Yaml.h"
// 仮
#include "ApproachState.h"
#include "GpuParticle.h"
#include "SpawnManager.h"

#include "ISceneState.h"
#include "game/state/PauseState.h"

#include "SpriteMotionLibrary.h"

///=============================================================================
///						マトリックス表示
static void ShowMatrix4x4(const Kouro::Matrix4x4 &matrix, const char *label) {
	ImGui::Text("%s", label);
	if(ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// 
		for(int i = 0; i < 4; ++i) {
			ImGui::TableNextRow();
			for(int j = 0; j < 4; ++j) {
				ImGui::TableSetColumnIndex(j);
				ImGui::Text("%.3f", matrix.m[i][j]);
			}
		}
		ImGui::EndTable();
	}
}

///=============================================================================
///						初期化
void GameScene::Initialize(Kouro::EngineContext context) {
	//========================================
	// 基底シーン
	BaseScene::Initialize(context);

	// postエフェクトの適用
	sceneManager_->GetPostEffect()->ApplyEffect("dissolve",Kouro::PostEffect::EffectType::Dissolve);

	// YAMLファイルの読み込み
	YAML::Node config = KOURO::YamlLoader::LoadYamlFile("game/rail_config.yaml");

	// コントロールポイントの読み込み
	const std::vector<Kouro::Vector3>& controlPoints_ = config["control_points"].as<std::vector<Kouro::Vector3>>();

	Rail rail;

	rail.Initialize(controlPoints_);

	enemyRail_.Initialize(rail,60.0f);
	playerRail_.Initialize(rail, 60.0f);
	cameraRail_.Initialize(rail, 60.0f);
	
	enemyRail_.SetDistanceTravelled(60.0f);
	playerRail_.SetDistanceTravelled(30.0f);
	cameraRail_.SetDistanceTravelled(0.0f);

	cameraRail_.SetLookAheadDistance(20.0f);

	enemyRail_.Update(0.0f);
	playerRail_.Update(0.0f);
	cameraRail_.Update(0.0f);

	///========================================
	///		ライン描画
	lineDrawer_ = std::make_unique<Kouro::LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(),sceneManager_->GetSrvManager());

	lineModelManager_ = std::make_unique<Kouro::LineModelManager>();
	lineModelManager_->Initialize(lineDrawer_.get());
	lineModelManager_->LoadLineModel("player/player.obj");
	lineModelManager_->LoadLineModel("enemy/enemy.obj");
	lineModelManager_->LoadLineModel("playerbullet/playerbullet.obj");
	lineModelManager_->LoadLineModel("stage.obj");

	transform_ = std::make_unique<Kouro::WorldTransform>();
	transform_->Initialize();
	transform_->SetScale({ 300.0f,300.0f,100.0f });

	///========================================
	///		パーティクル
	
	// plane
	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("plane_Particle", "texture/circle.png", Kouro::ParticleManager::ParticleType::Normal);
	// ブレンドモードの設定
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	// billboardを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableBillboard = true;

	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->flagsData->enableStretch = true;
	
	// 減速を有効



	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enableDeceleration = false;
	// パルスを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enablePulse = false;



	emitter1_ = std::make_unique<Kouro::ParticleEmitter>();
	emitter1_->Initialize("plane_Particle");
	emitter1_->SetPosition({ 1.0f,2.0f,0.0f });
	emitter1_->SetParticleCount(1);
	emitter1_->SetVelocityRange({ {0.0f,0.0f,2.0f },{0.0f,0.0f,2.0f} });
	emitter1_->SetStartScaleRange({ {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} });

	emitter1_->SetFinishScaleRange({ {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} });

	emitter1_->SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

	emitter1_->SetStartColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} });
	emitter1_->SetFinishColorRange({ {0.667f, 0.169f, 1.0f, 1.0f},{0.667f, 0.169f, 1.0f, 1.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
	emitter1_->SetLifeTimeRange({ 10.0f,10.0f });
	emitter1_->SetFrequency(0.07f);

	mEmitter = std::make_unique<Kouro::ModelEdgeEmitter>();
	mEmitter->Initialize("normal", context);
	mEmitter->CreateLineSegment("enemy/enemy.obj");

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("HitEffect", "texture/gradationLine.png", Kouro::ParticleManager::ParticleType::Ring);

	// ブレンドモードの設定
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	// billboardを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->flagsData->enableBillboard = false;
	// 減速を有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->enableDeceleration = true;
	// パルスを有効
	emitter2_ = std::make_unique<Kouro::ParticleEmitter>();
	emitter2_->Initialize("HitEffect");
	emitter2_->SetPosition({ 1.0f,2.0f,20.0f });
	emitter2_->SetParticleCount(1);
	emitter2_->SetVelocityRange({ {0.0f,0.0f,0.0f },{0.0f,0.0f,0.0f} });
	emitter2_->SetStartScaleRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	emitter2_->SetFinishScaleRange({ {3.0f,3.0f,3.0f},{3.0f,3.0f,3.0f} });
	emitter2_->SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
	

	emitter2_->SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

	emitter2_->SetStartColorRange({ {1.0f,1.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f} });
	emitter2_->SetFinishColorRange({ {1.0f,1.0f,1.0f,0.0f},{1.0f,1.0f,1.0f,0.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
	emitter2_->SetLifeTimeRange({ 0.5f,0.5f });
	emitter2_->SetFrequency(0.2f);

	///hitEffect1

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("spark", "texture/circle.png", Kouro::ParticleManager::ParticleType::Normal);

	Kouro::ParticleManager::GetInstance()->GetParticleGroup("spark")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("spark")->flagsData->enableBillboard = true;
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("spark")->flagsData->enableStretch = false;

	emitter3_ = std::make_unique<Kouro::ParticleEmitter>();
	emitter3_->Initialize("spark");
	emitter3_->SetPosition({ 1.0f,2.0f,20.0f });
	emitter3_->SetParticleCount(10);
	emitter3_->SetVelocityRange({ {-10.0f,-10.0f,-10.0f },{10.0f,10.0f,10.0f} });
	emitter3_->SetStartScaleRange({ {0.3f,0.3f,0.3f},{0.3f,0.3f,0.3f} });
	emitter3_->SetFinishScaleRange({ {0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f} });
	emitter3_->SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });


	emitter3_->SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

	emitter3_->SetStartColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} });
	emitter3_->SetFinishColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
	emitter3_->SetLifeTimeRange({ 0.5f,0.5f });
	emitter3_->SetFrequency(0.01f);

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("ray", "texture/circle.png", Kouro::ParticleManager::ParticleType::Normal);

	Kouro::ParticleManager::GetInstance()->GetParticleGroup("ray")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("ray")->flagsData->enableBillboard = true;
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("ray")->flagsData->enableStretch = true;

	emitter4_ = std::make_unique<Kouro::ParticleEmitter>();
	emitter4_->Initialize("ray");
	emitter4_->SetPosition({ 1.0f,2.0f,20.0f });
	emitter4_->SetParticleCount(10);
	emitter4_->SetVelocityRange({ {-10.0f,-10.0f,-10.0f },{10.0f,10.0f,10.0f} });
	emitter4_->SetStartScaleRange({ {0.3f,0.3f,0.3f},{0.3f,0.3f,0.3f} });
	emitter4_->SetFinishScaleRange({ {0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f} });
	emitter4_->SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });


	emitter4_->SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

	emitter4_->SetStartColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} });
	emitter4_->SetFinishColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
	emitter4_->SetLifeTimeRange({ 0.5f,0.5f });
	emitter4_->SetFrequency(0.01f);


	///========================================
	///		プレイヤー
	
	// 初期化と生成
	player_ = std::make_shared<Player>();
	player_->Initialize(lineModelManager_->FindLineModel("player/player.obj"));
	player_->SetLineModelManager(lineModelManager_.get());

	///========================================
	/// 入力

	// マウスの入力受付を有効
	Kouro::Input::GetInstance()->SetIsReception(true);

	std::unique_ptr<RailCamera> camera = std::make_unique<RailCamera>();
	camera->Initialize();
	camera->SetParent(&cameraRail_.GetWorldTransform());
	camera->SetTarget(player_->GetWorldTransform());
	railCamera_ = camera.get();
	cameraManager_->AddCamera("railCamera", std::move(camera));
	cameraManager_->SetActiveCamera("railCamera");

	player_->SetParentTransform(&playerRail_.GetWorldTransform());

	colliderManager_ = std::make_unique<ColliderManager>();

	for(uint32_t i = 0; i < 3; ++i)
	{
		countSprite_[i] = std::make_unique<Kouro::Sprite>();

		std::string filePath = "texture/" + std::to_string(i + 1) + ".png";

		Kouro::TextureManager::GetInstance()->LoadTexture(filePath);

		countSprite_[i]->Initialize(Kouro::SpriteCommon::GetInstance(), filePath);
		countSprite_[i]->SetSize({ 360.0f,360.0f });
		countSprite_[i]->SetPosition({ 640.0f ,360.0f });
		countSprite_[i]->SetTexSize({ 1536.0f,1024.0f });

		countSprite_[i]->SetAnchorPoint({ 0.5f,0.5f });
		countSprite_[i]->Update();
	}

	spriteManager_->LoadSpriteGroupsFromYaml("game/ui/scene_ui.yaml");

	// プレイUIの設定
	spriteManager_->SetGroupVisibility("play_ui", true);

	// 発砲UIの更新関数を設定
	spriteManager_->SetSpriteUpdateFunction("play_ui","label_fire_hint",PlayerUI::FireUI(kDefaultUIColor_,{ 1.0,1.0f,1.0f,1.0f },player_.get()));

	// ポーズUIの設定
	spriteManager_->SetGroupVisibility("pause_ui", false);

	colliderManager_->AddCollider(player_);

	player_->SetColliderManager(colliderManager_.get());

	fade_->Start(Fade::Status::WhiteFadeIn, 1.0f);

	sceneManager_->GetPostEffect()->ApplyEffect("blur", Kouro::PostEffect::EffectType::RadialBlur);

	stage_ = std::make_unique<Kouro::ObjectLine>();
	stage_->Initialize(lineModelManager_->FindLineModel("stage.obj"));

	SpawnManager spawnManager;

	std::vector<Kouro::Vector3> enemies = spawnManager.LoadFile("config/game/spawnPattern1.json");

	for (size_t i = 0; i < enemies.size(); i++)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemy->Initialize(lineModelManager_->FindLineModel("enemy/enemy.obj"));

		enemy->SetGoalOffset(enemies[i]);

		enemy->SetTarget(player_.get());

		enemy->SetParent(&enemyRail_.GetWorldTransform());

		enemy->SetColliderManager(colliderManager_.get());

		enemy->SetLineModelManager(lineModelManager_.get());

		enemy->SetEmitter(mEmitter.get());

		std::unique_ptr<EnemyState> state = std::make_unique<ApproachState>();

		enemy->SetCameraManager(cameraManager_.get());

		enemy->ChangeState(std::move(state));

		colliderManager_->AddCollider(enemy);

		enemy->Update();

		enemies_.push_back(std::move(enemy));
	}

	

	pointEmitter_ = std::make_unique<Kouro::PointEmitter>();
	pointEmitter_->Initialize("normal", context);
	pointEmitter_->SetEmitterProperties({ 0.0f,0.0f,0.0f }, 100 ,{ -4.0f,-4.0f,-10.0f }, { 4.0f,4.0f,-1.0f }, 0.5f, 1.5f, 0.0f);
	//pointEmitter_->SetEmitterProperties({ 0.0f,0.0f,0.0f }, 1, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 100.0f, 150.0f, 0.0f);
	pointEmitter_->GetWorldTransform()->SetParent(player_->GetWorldTransform());
}

///=============================================================================
///						終了処理
void GameScene::Finalize()
{
	BaseScene::Finalize();
}

///=============================================================================
///						更新
void GameScene::Update()
{
	// 基底クラスの更新
	BaseScene::Update();

	// パーティクルマネージャーの更新
	Kouro::ParticleManager::GetInstance()->Update();

	// postEffectの処理
	Kouro::RadialBlur* blur = static_cast<Kouro::RadialBlur*>(sceneManager_->GetPostEffect()->GetEffectData("blur"));
	Kouro::Radial::Material material = blur->GetMaterial();

	const QuickMoveData* data = player_->GetQuickMoveData();
	
	if (state_)
	{
		state_->Update();
	}

	if (data->isQuickMoving)
	{
		if (data->actionTimer < data->duration)
		{
			// 経過割合 (1.0f → 0.0f) に反転
			float t = 1.0f - (data->actionTimer / data->duration);

			// サインカーブを使って自然な減衰に（始め強く→ゆっくり消える）
			float blurIntensity = sinf(t * 3.14159f * 0.5f); // 半サイン波でふわっと減衰
			blurIntensity = std::clamp(blurIntensity, 0.0f, 1.0f);

			// ブラー設定（全体的に控えめ）
			material.blurWidth = 0.01f + 0.03f * blurIntensity; // 0.01〜0.04程度
			material.numSamples = 2 + int(blurIntensity * 3);    // 2〜5サンプル
		}
		else
		{
			// 終了時リセット
			material.blurWidth = 0.0f;
			material.numSamples = 1;
		}
	}
	else
	{
		material.blurWidth = 0.0f;
		material.numSamples = 1;
	}
	
	blur->SetMaterialData(material);

	transform_->UpdateMatrix();

	// プレイヤーが死亡していたら
	if (!player_->GetIsAlive())
	{
		// Dissolveエフェクトの取得
		auto* dissolve = static_cast<Kouro::Dissolve*>(sceneManager_->GetPostEffect()->GetEffectData("dissolve"));
		dissolve->edgeColor = { 1.0f,0.0f,0.0f };
		dissolve->thresholdWidth = 0.25f;

		k += 0.01f;

		// Dissolveエフェクトの閾値をイージングで増加させる
		dissolve->threshold = Kouro::Easing::EaseInQuad(k);

		// 閾値が1.0f以上になったらシーンをゲームオーバーに変更
		if (dissolve->threshold >= 1.0f)
		{
			sceneManager_->ChangeScene("OVER");
		}
	}

	// 敵の数を取得
	size_t enemyCount = enemies_.size();

	switch (phase_)
	{
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{

			countTimer_ += deltaTime;

			if (countTimer_ < 1.0f)
			{
				countSprite_[2]->SetSize({ 360.0f * (1.0f + countTimer_),360.0f * (1.0f + countTimer_) });
				countSprite_[2]->SetColor({ 1.0f,1.0f,1.0f,1.0f - countTimer_ });
				countSprite_[2]->SetRotation(countSprite_[2]->GetRotation() + deltaRotation);
				countSprite_[2]->Update();
			}
			else if (countTimer_ < 2.0f)
			{
				countSprite_[1]->SetSize({ 360.0f * (1.0f + countTimer_ - 1.0f),360.0f * (1.0f + countTimer_ - 1.0f) });
				countSprite_[1]->SetColor({ 1.0f,1.0f,1.0f,1.0f - countTimer_ + 1.0f });
				countSprite_[1]->SetRotation(countSprite_[1]->GetRotation() + deltaRotation);
				countSprite_[1]->Update();
			}
			else if (countTimer_ < 3.0f)
			{
				countSprite_[0]->SetSize({ 360.0f * (1.0f + countTimer_ - 2.0f),360.0f * (1.0f + countTimer_ - 2.0f) });
				countSprite_[0]->SetColor({ 1.0f,1.0f,1.0f,1.0f - countTimer_ + 2.0f });
				countSprite_[0]->SetRotation(countSprite_[0]->GetRotation() + deltaRotation);
				countSprite_[0]->Update();
			}

			else if (countTimer_ >= 3.0f)
			{
				Kouro::Input::GetInstance()->SetIsReception(true);

				phase_ = Phase::kMain;

				railCamera_->SetIsMove(true);
			}
		}

		break;
	case Phase::kMain:

		if (Kouro::Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			std::unique_ptr<PauseState> newState = std::make_unique<PauseState>();

			newState->SetOnExitCallback([this](const std::string& result) {OnPauseExit(result); });

			ChangeState(std::move(newState));

			// ポーズシーンへ
			phase_ = Phase::kPose;
			spriteManager_->SetGroupVisibility("pause_ui", true);
			return;
		}

		UpdateAllObjects(1.0f / 60.0f);

		for (auto& enemy : enemies_)
		{
			if (!enemy->GetIsAlive())
			{
				++eliminatedEnemyCount_;
			}
		}

		// 敵が全滅したらフェードアウトへ
		if (enemyCount == 0)
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			spriteManager_->SetGroupVisibility("pause_ui", false);
			phase_ = Phase::kFadeOut;
		}

		pointEmitter_->Update();
		pointEmitter_->Emit(player_->GetWorldTransform()->GetWorldMatrix());

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
			if(isBackToTitle_)
			{
				sceneManager_->ChangeScene("TITLE");
				return;
			}

			// 敵の数が0ならクリアシーンへ
			if(enemyCount == 0)
			{
				globalVariables_.SaveFile("ELIMINATED_ENEMY_COUNT", eliminatedEnemyCount_);

				sceneManager_->ChangeScene("CLEAR");

				return;
			}

			Kouro::SceneManager::GetInstance()->ChangeScene("OVER");

			return;
		}

		break;

	case Phase::kPlay:
		break;
	case Phase::kPose:

		if (Kouro::Input::GetInstance()->TriggerKey(DIK_ESCAPE))
		{
			// ポーズシーンへ
			phase_ = Phase::kMain;
			spriteManager_->SetGroupVisibility("pause_ui", false);
			return;
		}
		
		break;
	}

	spriteManager_->UpdateVisibleGroups();

	if (player_->GetIsHit())
	{
		cameraManager_->CameraShake(0.5f);
	}
}

///=============================================================================
///						描画
void GameScene::Draw() 
{
	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画

	// 線描画前設定
	lineDrawer_->PreDraw(cameraManager_->GetActiveCamera()->GetViewProjection());

	player_->Draw();

	// 生存していない敵をリストから削除
	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
		[](const std::shared_ptr<Enemy>& enemy) { return !enemy->GetIsAlive(); }),
		enemies_.end());

	if (phase_ != Phase::kFadeIn)
	{
		// 敵の描画
		for (auto& enemy : enemies_)
		{
			enemy->Draw();
		}
	}

	// ステージの描画
	stage_->Draw(transform_.get());

	DrawForegroundSprite();
	/// 前景スプライト描画

	if (countTimer_ < 1.0f && countTimer_ > 0.0f)
	{
		countSprite_[2]->Draw();
	}
	else if (countTimer_ < 2.0f && countTimer_ > 1.0f)
	{
		countSprite_[1]->Draw();
	}
	else if (countTimer_ < 3.0f && countTimer_ > 2.0f)
	{
		countSprite_[0]->Draw();
	}

	spriteManager_->DrawGroup("play_ui");
	spriteManager_->DrawGroup("pause_ui");

	// フェード描画
	DrawFade();

	//========================================
	//パーティクルの描画
	Kouro::ParticleManager::GetInstance()->Draw("texture/circle.png");
}

void GameScene::UpdateAllObjects(const float deltaTime)
{
	playerRail_.Update(deltaTime);
	enemyRail_.Update(deltaTime);
	cameraRail_.Update(deltaTime);

	player_->Update();

	for (auto& enemy : enemies_)
	{
		enemy->Update();
	}

	colliderManager_->Update();

	// 生存していない敵をリストから削除
	std::erase_if(enemies_, [](const std::shared_ptr<Enemy>& enemy) {return !enemy->GetIsAlive(); });
}

void GameScene::ChangeState(std::unique_ptr<Kouro::ISceneState> newState)
{
	if (state_)
	{
		state_->OnExit();
	}

	state_ = std::move(newState);

	if (state_)
	{
		state_->OnEnter(this);
	}
}

void GameScene::OnPauseExit(const std::string& result)
{
	if (result == SceneCommand::Resume)
	{
		// ポーズ解除 → ゲーム再開
		/*ChangeState(std::make_unique<>());*/
		ResetState();
	}
	else if (result == SceneCommand::Title) {
		// タイトルシーンへ
		sceneManager_->ChangeScene(SceneCommand::Title);
	}
}
