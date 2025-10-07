#include "GameScene.h"
#include "imgui.h"
#include <imgui_internal.h>

// 仮
#include "ApproachState.h"

///=============================================================================
///						マトリックス表示
static void ShowMatrix4x4(const Matrix4x4 &matrix, const char *label) {
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
void GameScene::Initialize() {
	//========================================
	// 基底シーン
	BaseScene::Initialize();

	//========================================
	// テクスチャの読み込み
	
	ModelManager::GetInstance()->LoadModel("terrain.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");
	ModelManager::GetInstance()->LoadModel("playerbullet/playerbullet.obj");

	//========================================
	// ライト
	// 指向性
	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();
	directionalLight->intensity_ = 0.0f;
	// 点光源
	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();
	pointLight->intensity_ = 0.0f;
	// スポットライト
	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();
	spotLight->direction_ = { 0.0f,-1.0f,0.0f };
	spotLight->position_ = { 0.0f,10.0f,0.0f };
	spotLight->intensity_ = 1.0f;
	spotLight->decay_ = 0.87f;
	spotLight->distance_ = 40.0f;
	spotLight->cosAngle_ = 0.5f;

	//========================================
	// アクティブカメラをフォローカメラに設定
	//cameraManager_->useDebugCamera_ = true;

	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Grayscale); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Vignette); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::BoxFilter); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::GaussianFilter); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::LuminanceBasedOutline); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::DepthBasedOutline); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::RadialBlur); //完
	//sceneManager_->GetPostEffect()->ApplyEffect("dissolve",PostEffect::EffectType::Dissolve); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Random); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::LinearFog); //完
	//sceneManager_->GetPostEffect()->ApplyEffect("Blur",PostEffect::EffectType::MotionBlur);
	sceneManager_->GetPostEffect()->ApplyEffect("colorSpace",PostEffect::EffectType::ColorSpace);

	///========================================
	///	    アニメーション
	animationManager = std::make_unique<AnimationManager>();
	animationManager->LoadAnimationFile("./Resources/human", "walk.gltf");
	animationManager->StartAnimation("walk.gltf",0);

	///========================================
	///		ライン描画
	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(),sceneManager_->GetSrvManager());
	//lineDrawer_->CreateLineObject(LineDrawerBase::Type::Grid, nullptr);
	//lineDrawer_->CreateSkeletonObject(animationManager->GetActiveAnimation("walk.gltf").skeleton,nullptr);

	ModelManager::GetInstance()->LoadModel("terrain.obj");

	ground_ = std::make_unique<Object3d>();
	ground_->Initialize(Object3dCommon::GetInstance());
	ground_->SetModel(ModelManager::GetInstance()->FindModel("terrain.obj"));
	ground_->SetLocalMatrix(MakeIdentity4x4());

	transform_ = std::make_unique<WorldTransform>();
	transform_->Initialize();

	///========================================
	///		パーティクル
	
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



	emitter1_ = std::make_unique<ParticleEmitter>();
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
	

	ParticleManager::GetInstance()->CreateParticleGroup("HitEffect", "Resources/gradationLine.png", ParticleManager::ParticleType::Ring);

	// ブレンドモードの設定
	ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->blendMode = ParticleManager::BlendMode::kAdd;
	// billboardを有効
	ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->flagsData->enableBillboard = false;
	// 減速を有効
	ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->enableDeceleration = true;
	// パルスを有効
	//ParticleManager::GetInstance()->GetParticleGroup("smoke")->enablePulse = true;
	emitter2_ = std::make_unique<ParticleEmitter>();
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

	ParticleManager::GetInstance()->CreateParticleGroup("spark", "Resources/circle.png", ParticleManager::ParticleType::Normal);

	ParticleManager::GetInstance()->GetParticleGroup("spark")->blendMode = ParticleManager::BlendMode::kAdd;
	ParticleManager::GetInstance()->GetParticleGroup("spark")->flagsData->enableBillboard = true;
	ParticleManager::GetInstance()->GetParticleGroup("spark")->flagsData->enableStretch = false;

	emitter3_ = std::make_unique<ParticleEmitter>();
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
	
	ParticleManager::GetInstance()->CreateParticleGroup("ray", "Resources/circle.png", ParticleManager::ParticleType::Normal);

	ParticleManager::GetInstance()->GetParticleGroup("ray")->blendMode = ParticleManager::BlendMode::kAdd;
	ParticleManager::GetInstance()->GetParticleGroup("ray")->flagsData->enableBillboard = true;
	ParticleManager::GetInstance()->GetParticleGroup("ray")->flagsData->enableStretch = true;

	emitter4_ = std::make_unique<ParticleEmitter>();
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
	player_->Initialize((ModelManager::GetInstance()->FindModel("player/player.obj")));

	///========================================
	/// 入力

	// マウスの入力受付を有効
	Input::GetInstance()->SetIsReception(true);

	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize();

	cameraManager_->SetActiveCamera(railCamera_.get());
    //cameraManager_->useDebugCamera_ = true;
	cameraManager_->Update();

	player_->SetCamera(cameraManager_->GetActiveCamera());

	player_->SetParentTransform(railCamera_->GetWorldTransform());


	LevelLoader loader;

	levelData_ = loader.LoadLevelFromJson("Resources/TL1.json");

	for (auto& object : levelData_->objects)
	{
		object.object3d->SetLocalMatrix(MakeIdentity4x4());
	}

	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");

	colliderManager_ = std::make_unique<ColliderManager>();

	for (int i = 0; i < 4; ++i)
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemy->Initialize(ModelManager::GetInstance()->FindModel("enemy/enemy.obj"));

		enemy->SetCamera(cameraManager_->GetActiveCamera());

		enemy->SetTarget(player_.get());

		enemy->SetColliderManager(colliderManager_.get());

		std::unique_ptr<EnemyState> state = std::make_unique<ApproachState>();

		enemy->ChangeState(std::move(state));

		enemy->SetPosition(Vector3(0.0f, 1.0f, 5 + i * 40.0f));

		colliderManager_->AddCollider(enemy);

		enemies_.push_back(std::move(enemy));
	}

	colliderManager_->AddCollider(player_);

	player_->SetColliderManager(colliderManager_.get());

	lineDrawer_->CreateCatmullRomLine(railCamera_->GetControlPoints(), nullptr);

	lineDrawer_->CreateObject3DLine("player/player.obj",player_->GetWorldTransform());

	//lineDrawer_->CreateObject3DLine("playerbullet/playerbullet.obj", player_->GetWorldTransform());
}
///=============================================================================
///						終了処理
void GameScene::Finalize()
{

	BaseScene::Finalize();

}
///=============================================================================
///						更新
void GameScene::Update() {
	animationManager->Update();
	lineDrawer_->SkeletonUpdate(animationManager->GetActiveAnimation("walk.gltf").skeleton);
	//human_->Update();

	BaseScene::Update();

	//emitter1_->Update();

	//emitter2_->Update();

	ParticleManager::GetInstance()->Update();

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

	ground_->Update();

	transform_->UpdateMatrix();

	player_->Update();

	for (auto& object : levelData_->objects)
	{
		object.worldTransform->UpdateMatrix();
	}

	if (Input::GetInstance()->Triggerkey(DIK_E))
	{
		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
		enemy->Initialize(ModelManager::GetInstance()->FindModel("enemy/enemy.obj"));

		enemy->SetCamera(cameraManager_->GetActiveCamera());

		enemy->SetTarget(player_.get());

		enemy->SetColliderManager(colliderManager_.get());

		std::unique_ptr<EnemyState> state = std::make_unique<ApproachState>();

		enemy->ChangeState(std::move(state));

		colliderManager_->AddCollider(enemy);

		enemies_.push_back(std::move(enemy));
	}

	for (auto& enemy : enemies_)
	{
		enemy->Update();
	}

	size_t enemyCount = enemies_.size();

	switch (phase_)
	{
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{
			Input::GetInstance()->SetIsReception(true);
			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if (enemyCount == 0)
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		if (Input::GetInstance()->PushKey(DIK_SPACE))
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
			SceneManager::GetInstance()->ChangeScene("TITLE");

			return;
		}

		break;

	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	std::erase_if(enemies_, [](const std::shared_ptr<Enemy>& enemy) {
		return !enemy->GetIsAlive();
		});

	colliderManager_->Update();

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
	
#endif

	//emitter1_->Update();

	//emitter2_->Update();

	//emitter3_->Update();

	//emitter4_->Update();

	railCamera_->Update();

	lineDrawer_->Update();
}

///=============================================================================
///						描画
void GameScene::Draw() 
{
	skybox_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画

	//ground_->Draw(*transform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	for (auto &object : levelData_->objects)
	{
		object.object3d->Draw(*object.worldTransform.get(), cameraManager_->GetActiveCamera()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());
	}

	player_->Draw(*directionalLight.get(), *pointLight.get(), *spotLight.get());

	for (auto& enemy : enemies_)
	{
		enemy->Draw(*directionalLight.get(), *pointLight.get(), *spotLight.get());
	}

	DrawForegroundSprite();
	/// 前景スプライト描画

	// フェード描画
	DrawFade();

	//========================================
	//パーティクルの描画
	ParticleManager::GetInstance()->Draw("Resources/circle.png");	

	lineDrawer_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection());
}