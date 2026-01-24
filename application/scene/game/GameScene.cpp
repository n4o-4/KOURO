#include "GameScene.h"
#include "imgui.h"
#include <imgui_internal.h>

#include "YamlLoader.h"
// 仮
#include "ApproachState.h"
#include "Easing.h"
#include "GpuParticle.h"
#include "SpawnManager.h"

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

	//========================================
	// テクスチャの読み込み

	Kouro::TextureManager::GetInstance()->LoadTexture("Resources/WASD.png");


	Kouro::ModelManager::GetInstance()->LoadModel("player/player.obj");
	Kouro::ModelManager::GetInstance()->LoadModel("playerbullet/playerbullet.obj");

	//========================================
	// ライト
	// 指向性
	directionalLight = std::make_unique<Kouro::DirectionalLight>();
	directionalLight->Initialize();
	directionalLight->intensity_ = 0.0f;
	// 点光源
	pointLight = std::make_unique<Kouro::PointLight>();
	pointLight->Initilize();
	pointLight->intensity_ = 0.0f;
	// スポットライト
	spotLight = std::make_unique<Kouro::SpotLight>();
	spotLight->Initialize();
	spotLight->direction_ = { 0.0f,-1.0f,0.0f };
	spotLight->position_ = { 0.0f,10.0f,0.0f };
	spotLight->intensity_ = 1.0f;
	spotLight->decay_ = 0.87f;
	spotLight->distance_ = 40.0f;
	spotLight->cosAngle_ = 0.5f;

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
	
	enemyRail_.SetDistanceTravelled(20.0f);
	playerRail_.SetDistanceTravelled(20.0f);
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
	transform_->transform.translate = { 0.0f,0.0f,0.0f };

	transform_->transform.scale = { 300.0f,300.0f,100.0f };
	///========================================
	///		パーティクル
	
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

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("HitEffect", "Resources/gradationLine.png", Kouro::ParticleManager::ParticleType::Ring);

	// ブレンドモードの設定
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->blendMode = Kouro::ParticleManager::BlendMode::kAdd;
	// billboardを有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->flagsData->enableBillboard = false;
	// 減速を有効
	Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->enableDeceleration = true;
	// パルスを有効
	//Kouro::ParticleManager::GetInstance()->GetParticleGroup("smoke")->enablePulse = true;
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

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("spark", "Resources/circle.png", Kouro::ParticleManager::ParticleType::Normal);

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

	Kouro::ParticleManager::GetInstance()->CreateParticleGroup("ray", "Resources/circle.png", Kouro::ParticleManager::ParticleType::Normal);

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

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize();
	railCamera_->SetParent(&cameraRail_.GetWorldTransform());

	

	cameraManager_->CamerasClear();
	cameraManager_->SetActiveCamera(railCamera_.get());
	cameraManager_->Update();
	//cameraManager_->GetActiveCamera()->GetWorldTransform().SetTranslate({0.0f,0.0f,280.0f });
	player_->SetParentTransform(&playerRail_.GetWorldTransform());


	Kouro::LevelLoader loader;

	levelData_ = loader.LoadLevelFromJson("Resources/TL1.json");

	for (auto& object : levelData_->objects)
	{
		object.object3d->SetLocalMatrix(Kouro::MakeIdentity4x4());
	}

	Kouro::ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");

	colliderManager_ = std::make_unique<ColliderManager>();

	for(uint32_t i = 0; i < 3; ++i)
	{
		countSprite_[i] = std::make_unique<Kouro::Sprite>();

		std::string filePath = "Resources/" + std::to_string(i + 1) + ".png";

		Kouro::TextureManager::GetInstance()->LoadTexture(filePath);

		countSprite_[i]->Initialize(Kouro::SpriteCommon::GetInstance(), filePath);
		countSprite_[i]->SetSize({ 360.0f,360.0f });
		countSprite_[i]->SetPosition({ 640.0f ,360.0f });
		countSprite_[i]->SetTexSize({ 1536.0f,1024.0f });

		countSprite_[i]->SetAnchorPoint({ 0.5f,0.5f });
		countSprite_[i]->Update();
	}

	WASD_ = std::make_unique<Kouro::Sprite>();
	WASD_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/WASD.png");
	WASD_->SetAnchorPoint(Kouro::Vector2(0.5f, 0.5f));
	WASD_->SetTexSize(Kouro::Vector2(1024.0f, 1024.0f));
	WASD_->SetSize(Kouro::Vector2(200.0f, 200.0f));
	WASD_->SetPosition(Kouro::Vector2(120.0f, 600.0f));
	WASD_->SetColor(Kouro::Vector4(kDefaultUIColor_));
	WASD_->Update();

	fireUI_ = std::make_unique<Kouro::Sprite>();
	fireUI_->Initialize(Kouro::SpriteCommon::GetInstance(), "Resources/FireUI.png");
	fireUI_->SetAnchorPoint(Kouro::Vector2(0.5f, 0.5f));
	fireUI_->SetTexSize(Kouro::Vector2(1536.0f, 1024.0f));
	fireUI_->SetSize(Kouro::Vector2(300.0f, 200.0f));
	fireUI_->SetPosition(Kouro::Vector2(640.0f, 600.0f));
	fireUI_->SetColor(Kouro::Vector4(kDefaultUIColor_));
	fireUI_->Update();

	colliderManager_->AddCollider(player_);

	player_->SetColliderManager(colliderManager_.get());

	fade_->Start(Fade::Status::WhiteFadeIn, 1.0f);

	sceneManager_->GetPostEffect()->ApplyEffect("blur", Kouro::PostEffect::EffectType::RadialBlur);

	stage_ = std::make_unique<Kouro::ObjectLine>();
	stage_->Initialize(lineModelManager_->FindLineModel("stage.obj"));

	SpawnManager spawnManager;

	std::vector<Kouro::Vector3> enemies = spawnManager.LoadFile("spawnPattern1.json");

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

		enemies_.push_back(std::move(enemy));
	}
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

	//GpuParticle::GetInstance()->Update(cameraManager_->GetActiveCamera()->GetViewProjection());

	// postEffectの処理
	Kouro::RadialBlur* blur = static_cast<Kouro::RadialBlur*>(sceneManager_->GetPostEffect()->GetEffectData("blur"));
	Kouro::Radial::Material material = blur->GetMaterial();

	const QuickMoveData* data = player_->GetQuickMoveData();
	

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

	//========================================
	// ライト
	// 
	//========================================
	// ディレクショナルライト
	directionalLight->Update();
	//========================================
	// ポイントライト
	pointLight->Update();
	//========================================
	// スポットライト
	spotLight->Update();


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

	for (auto& object : levelData_->objects)
	{
		object.worldTransform->UpdateMatrix();
	}

#ifdef _DEBUG

	if (Kouro::Input::GetInstance()->Triggerkey(DIK_E))
	{
		SpawnManager spawnManager;

		std::vector<Kouro::Vector3> enemies = spawnManager.LoadFile("spawnPattern1.json");

		for (size_t i = 0; i < enemies.size(); i++)
		{
			std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
			enemy->Initialize(lineModelManager_->FindLineModel("enemy/enemy.obj"));

			enemy->SetGoalOffset(enemies[i]);

			enemy->SetTarget(player_.get());

			enemy->SetColliderManager(colliderManager_.get());

			enemy->SetLineModelManager(lineModelManager_.get());

			enemy->SetEmitter(mEmitter.get());

			std::unique_ptr<EnemyState> state = std::make_unique<ApproachState>();

			enemy->SetCameraManager(cameraManager_.get());

			enemy->ChangeState(std::move(state));

			enemy->SetParent(&enemyRail_.GetWorldTransform());

			colliderManager_->AddCollider(enemy);

			enemies_.push_back(std::move(enemy));
		}
	}

#endif

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

		enemyRail_.Update(1.0f / 60.0f);
		playerRail_.Update(1.0f / 60.0f);
		cameraRail_.Update(1.0f / 60.0f);


		player_->Update();

		for (auto& enemy : enemies_)
		{
			enemy->Update();
		}
		
		colliderManager_->Update();

		// 生存していない敵をリストから削除
		std::erase_if(enemies_, [](const std::shared_ptr<Enemy>& enemy) {
			return !enemy->GetIsAlive();
			});

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
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
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
		break;
	}

	

	if (player_->GetIsHit())
	{
		cameraManager_->CameraShake(0.5f);
	}

#ifdef _DEBUG

	if(ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("directionalLight.color", &directionalLight->color_.x, ImGuiColorEditFlags_None);
		if(ImGui::DragFloat3("directionalLight.direction", &directionalLight->direction_.x, 0.01f)) {
			directionalLight->direction_ = Normalize(directionalLight->direction_);
		}
		ImGui::DragFloat("directionalLight.intensity", &directionalLight->intensity_, 0.01f);
		ImGui::TreePop();
	}

	if(ImGui::TreeNode("pointLight")) {
		ImGui::ColorEdit4("pointLight.color", &pointLight->color_.x, ImGuiColorEditFlags_None);
		ImGui::DragFloat3("pointLight.position", &pointLight->position_.x, 0.01f);
		ImGui::DragFloat("pointLight.decay", &pointLight->decay_, 0.01f);
		ImGui::DragFloat("pointLight.radius", &pointLight->radius_, 0.01f);
		ImGui::DragFloat("pointLight.intensity", &pointLight->intensity_, 0.01f);
		ImGui::TreePop();
	}

	if(ImGui::TreeNode("spotLight")) {
		ImGui::ColorEdit4("spotlLight.color", &spotLight->color_.x, ImGuiColorEditFlags_None);
		if(ImGui::DragFloat3("spotLight.direction", &spotLight->direction_.x, 0.01f)) {
			spotLight->direction_ = Normalize(spotLight->direction_);
		}
		ImGui::DragFloat3("spotLight.position", &spotLight->position_.x, 0.01f);
		ImGui::DragFloat("spotLight.decay", &spotLight->decay_, 0.01f);
		ImGui::DragFloat("spotLight.intensity", &spotLight->intensity_, 0.01f);
		ImGui::DragFloat("spotLight.distance", &spotLight->distance_, 0.01f);
		ImGui::DragFloat("spotLight.cosAngle", &spotLight->cosAngle_, 0.01f);
		ImGui::DragFloat("spotLight.cosFalloffStart", &spotLight->cosFalloffStart_, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::Button("Emit"))
	{

		Kouro::Vector3 scale = { 1.0f,1.0f,1.0 };
		Kouro::Vector3 rotate = { 0.0f,0.0f,0.0f };
		Kouro::Vector3 translate = { 0.0f,0.0f,0.0f };

		Kouro::Matrix4x4 world = Kouro::MakeAffineMatrix(scale, rotate, translate);

		//GpuParticle::GetInstance()->LineEmit(world);
	}

#endif

	float interval = player_->GetFireInterval();
	float fireTimer = player_->GetFireIntervalTimer();

	float factor = fireTimer / interval;

	factor = std::clamp(factor, 0.0f, 1.0f);

	Kouro::Vector3 uiColor = Kouro::Lerp(Kouro::Vector3(kDefaultUIColor_.x, kDefaultUIColor_.y, kDefaultUIColor_.z), Kouro::Vector3(1.0f, 1.0f, 1.0f), factor);

	fireUI_->SetColor({ uiColor.x,uiColor.y,uiColor.z,1.0f });
	fireUI_->Update();

	//emitter1_->Update();

	//emitter2_->Update();

	//emitter3_->Update();

	//emitter4_->Update();

	//railCamera_->Update();
}

///=============================================================================
///						描画
void GameScene::Draw() 
{
	//skybox_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

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

	// 敵の描画
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
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

	WASD_->Draw();
	fireUI_->Draw();

	// フェード描画
	DrawFade();

	//========================================
	//パーティクルの描画
	Kouro::ParticleManager::GetInstance()->Draw("Resources/circle.png");
}