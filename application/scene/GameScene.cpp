﻿#include "GameScene.h"
#include "imgui.h"

///=============================================================================
///						マトリックス表示
void ShowMatrix4x4(const Matrix4x4 &matrix, const char *label) {
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
	// ライト
	// 指向性
	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();
	// 点光源
	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();
	// スポットライト
	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();
	//========================================
	// 天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize();
	//========================================
	// 地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();
	//========================================
	// プレイヤーを生成
	player_ = std::make_unique<Player>();
	player_->Initialize();
	//========================================
	// 🔽 LockOnを初期化
	lockOnSystem_ = std::make_unique<LockOn>();
	lockOnSystem_->Initialize();
	// 🔹 SetLockOnSystem() に std::move を使用
	player_->SetLockOnSystem(lockOnSystem_.get());  // 🔹 `std::move()` を使わず `get()` でポインタを渡す
	//========================================
	// 敵出現
	LoadEnemyPopData();

	//========================================
	// 当たり判定マネージャ
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(32.0f);

	//========================================
	// フォローカメラのターゲットを設定
	cameraManager_->SetFollowCameraTarget(player_->GetWorldTransform());

	//========================================
	// アクティブカメラをフォローカメラに設定
	cameraManager_->useFollowCamera_ = true;

	player_->SetFollowCamera(cameraManager_->GetFollowCamera());

	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Grayscale); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Vignette); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::BoxFilter); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::GaussianFilter); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::LuminanceBasedOutline); //完
	sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::DepthBasedOutline); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::RadialBlur); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Dissolve); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::Random); //完
	//sceneManager_->GetPostEffect()->ApplyEffect(PostEffect::EffectType::LinearFog); //完

	/*///========================================
	///		ライン描画
	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(),sceneManager_->GetSrvManager());


	///========================================
	/// アニメーションマネージャ
	animationManager = std::make_unique<AnimationManager>();	
	animationManager->LoadAnimationFile("./Resources/human", "walk.gltf");
	animationManager->StartAnimation("walk.gltf", 0);
	animationManager->Update();
	lineDrawer_->CreateSkeletonObject(animationManager->GetActiveAnimation("walk.gltf").skeleton,nullptr);*/

	///LoadModelでエラー発生中
	/*ModelManager::GetInstance()->LoadModel("human/wlak.gltf");	///

	human_ = std::make_unique<Object3d>();
	human_->Initialize(Object3dCommon::GetInstance());
	human_->SetModel(ModelManager::GetInstance()->FindModel("human/wlak.gltf"));*/

}
///=============================================================================
///						終了処理
void GameScene::Finalize() {
	skyDome_.reset();
	ground_.reset();

	player_->Finalize();
}
///=============================================================================
///						更新
void GameScene::Update() {
	/*animationManager->Update();
	lineDrawer_->SkeletonUpdate(animationManager->GetActiveAnimation("walk.gltf").skeleton);*/
	//human_->Update();
	//========================================
	// フェーズ切り替え
	switch(phase_) {
		//========================================
		// 
	case Phase::kFadeIn:

		if(fade_->IsFinished()) {

			phase_ = Phase::kPlay;

			// 入力の受付をオンに
			Input::GetInstance()->SetIsReception(true);
		}

		//---------------------------------------
		// プレイヤーの更新
		player_->Update();
		//---------------------------------------
		// 天球
		skyDome_->Update();
		//---------------------------------------
		// 地面
		ground_->Update();

		//========================================
		//
		break;
	case Phase::kPlay:

		//---------------------------------------
		// ゲームのクリア条件
		// クリアの場合
		// TODO: クリア条件を設定
		if(false) {
			isGameClear_ = true;
			isContinue_ = false;
		}
		// ゲームオーバーの場合
		if(player_->GetHp() <= 0) {
			isGameOver_ = true;
			isContinue_ = false;
		}
		// リセット
		if (Input::GetInstance()->Triggerkey(DIK_R))
		{
			isContinue_ = false;
		}
		if(!isContinue_) 
		{
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
		}
		//---------------------------------------
		// プレイヤーの更新
		player_->Update();
		//---------------------------------------
		// 天球
		skyDome_->Update();
		//---------------------------------------
		// 地面
		ground_->Update();
		//---------------------------------------
		// 敵出現
		UpdateEnemyPopCommands();
		//spawnの更新
		for (const auto &Spawn : spawns_) {
			Spawn->Update();
		}
		// 敵リスト
		for(const auto &enemy : enemies_) {
			enemy->Update();
		}
		// 敵の削除
		enemies_.erase(
			// 削除条件
			std::remove_if(enemies_.begin(), enemies_.end(),
				[this](const std::unique_ptr<BaseEnemy> &enemy) {
					// HPが0以下の場合
					if(enemy->GetHp() <= 0) {
						// ロックオンシステムから敵を削除
						if(lockOnSystem_) {
							lockOnSystem_->RemoveLockedEnemy(enemy.get());
						}
						return true; // 削除する
					}
					return false; // 削除しない
				}),
			// 実際に削除する
			enemies_.end());
		//---------------------------------------
		// ロックオンの処理追加
		if(lockOnSystem_) {
			// プレイヤーの位置をロックオンシステムにセット
			lockOnSystem_->SetPosition(player_->GetPosition());

			// カメラがFollowCameraの場合、視点方向を設定
			auto activeCamera = cameraManager_->GetActiveCamera();
			if(auto followCamera = dynamic_cast<FollowCamera *>( activeCamera )) {
				// カメラからの視点方向をロックオンシステムに設定
				lockOnSystem_->SetViewDirection(followCamera->GetForwardDirection());
			}
			// 敵の検出
			lockOnSystem_->DetectEnemies(enemies_);
			// ロックオン更新
			lockOnSystem_->Update(enemies_);
		}
		//---------------------------------------
		// 当たり判定
		// リセット
		collisionManager_->Reset();

		// エネミー
		for(auto &enemy : enemies_) {
			collisionManager_->AddCollider(enemy.get());
			// エネミーの弾リスト
			for(auto &bullet : enemy->GetBullets()) {
				collisionManager_->AddCollider(bullet.get());
			}
		}

		// プレイヤー
		collisionManager_->AddCollider(player_.get());

		// プレイヤーの弾リスト
		for(auto &bullet : player_->GetBullets()) {
			collisionManager_->AddCollider(bullet.get());
		}

		// プレイヤーのマシンガン弾リスト
		for (auto& machineGunBullet : player_->GetMachineGunBullets()) {
			collisionManager_->AddCollider(machineGunBullet.get());
		}

		// 更新
		collisionManager_->Update();
		
		//========================================
		// フェードアウト
		break;
	case Phase::kFadeOut:
		//---------------------------------------
		// `フェードアウトが終わった場合
		if(fade_->IsFinished()) {
			//ゲームをクリアした場合
			if(isGameClear_) {
				// ゲームクリアシーンに遷移
				SceneManager::GetInstance()->ChangeScene("CLEAR");
			}
			else if(isGameOver_) {
				// ゲームオーバーシーンに遷移
				SceneManager::GetInstance()->ChangeScene("OVER");
			} 
			else 
			{
				// タイトルシーンに遷移
				SceneManager::GetInstance()->ChangeScene("TITLE");
			}
		}

		//---------------------------------------
		// プレイヤーの更新
		player_->Update();

		//---------------------------------------
		// 天球
		skyDome_->Update();

		//---------------------------------------
		// 地面
		ground_->Update();

		//========================================
		// 

		break;
	case Phase::kMain:
		break;
	case Phase::kPose:
		break;
	}

	//lineDrawer_->Update();

	BaseScene::Update();
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
		ImGui::TreePop();
	}

	ImGui::Checkbox("useDebugCamera", &cameraManager_->useDebugCamera_);
	ImGui::Checkbox("sceneConticue", &isContinue);

#endif
}

///=============================================================================
///						描画
void GameScene::Draw() {

	switch(phase_) {
	case Phase::kFadeIn:

		DrawBackgroundSprite();
		/// 背景スプライト描画

		DrawObject();
		/// オブジェクト描画
		//========================================
		// 天球
		skyDome_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// 地面
		ground_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());

		DrawForegroundSprite();
		/// 前景スプライト描画	

		// フェード描画
		DrawFade();

		break;

	case Phase::kPlay:

		DrawBackgroundSprite();
		/// 背景スプライト描画

		DrawObject();
		/// オブジェクト描画
		//========================================
		// 天球
		skyDome_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// 地面
		ground_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// spawnの描画
		for (const auto& Spawn : spawns_) {
			Spawn->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
				*directionalLight.get(),
				*pointLight.get(),
				*spotLight.get());
		}
		//========================================
		// 敵
		for(const auto &enemy : enemies_) {
			enemy->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
				*directionalLight.get(),
				*pointLight.get(),
				*spotLight.get());
		}
		//========================================
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// LockOn
		// 🔽 LockOnの描画処理を追加
		if(lockOnSystem_) {
			lockOnSystem_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
				*directionalLight.get(),
				*pointLight.get(),
				*spotLight.get());
		}
		//========================================
		// 当たり判定マネージャ
		collisionManager_->Draw();

		DrawForegroundSprite();
		/// 前景スプライト描画	

		break;

	case Phase::kFadeOut:

		DrawBackgroundSprite();
		/// 背景スプライト描画

		DrawObject();
		/// オブジェクト描画
		//========================================
		// 天球
		skyDome_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// 地面
		ground_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// 敵
		for(const auto &enemy : enemies_) {
			enemy->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
				*directionalLight.get(),
				*pointLight.get(),
				*spotLight.get());
		}
		//========================================
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
		//========================================
		// LockOn
		 // 🔽 LockOnの描画処理を追加
		if(lockOnSystem_) {
			lockOnSystem_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
				*directionalLight.get(),
				*pointLight.get(),
				*spotLight.get());
		}
		//========================================
		// 当たり判定マネージャ
		collisionManager_->Draw();

		DrawForegroundSprite();
		/// 前景スプライト描画	

		// フェード描画
		DrawFade();

		break;

	case Phase::kMain:

		break;
	case Phase::kPose:

		break;
	}

	//lineDrawer_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection());
}

///=============================================================================
///                        静的メンバ関数
///--------------------------------------------------------------
///                        敵の出現データの読み込み
void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemySpawn.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}
///--------------------------------------------------------------
///                        敵の出現データの更新
void GameScene::UpdateEnemyPopCommands() {
	if(isWaiting_) {
		--waitTimer_;
		if(--waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		return;
	}
	std::string line;

	while(getline(enemyPopCommands, line)) {

		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if(word.find("//") == 0) {
			continue;
		}
		if(word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			SpawnSet(Vector3(x, y, z));

			getline(line_stream, word, ',');
			int num = std::stoi(word);

			getline(line_stream, word, ',');
			currentSpawnType_ = word;

			float spawnRangeX = 10.0f;
			float spawnRangeY = 5.0f;
			float spawnRangeZ = 10.0f;

			std::uniform_real_distribution<float> randomX(x - spawnRangeX, x + spawnRangeX);
			std::uniform_real_distribution<float> randomY(y - spawnRangeY, y + spawnRangeY);
			std::uniform_real_distribution<float> randomZ(z - spawnRangeZ, z + spawnRangeZ);

			for (int i = 0; i < num; i++) {
				if (currentSpawnType_ == "KUMO") {
					Vector3 randomPosition(randomX(gen), y, randomZ(gen));
					SpawnEnemyKumo(randomPosition);
				} else if (currentSpawnType_ == "BAT") {
					Vector3 randomPosition(randomX(gen), randomY(gen), randomZ(gen));
					SpawnEnemyBat(randomPosition);
				} else {
					SpawnEnemy(Vector3(x, y, z));
				}
			}
			
		} else if(word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaiting_ = true;
			waitTimer_ = waitTime;

			break;

		}


	}
}
///--------------------------------------------------------------
///                        敵の出現
void GameScene::SpawnEnemy(const Vector3 &position) {

	std::unique_ptr<BaseEnemy> newEnemy = std::make_unique<Enemy>();
	if (auto* enemyNormal = dynamic_cast<Enemy*>(newEnemy.get())) {
		enemyNormal->Initialize();
		enemyNormal->SetPosition(position);
		enemyNormal->SetTarget(player_->GetWorldTransform());
	}
	
	enemies_.push_back(std::move(newEnemy));

}

void GameScene::SpawnEnemyKumo(const Vector3& position) {
	std::unique_ptr<BaseEnemy> newEnemy = std::make_unique<GroundTypeEnemy>();
	if (auto* enemyNormal = dynamic_cast<GroundTypeEnemy*>(newEnemy.get())) {
		enemyNormal->Initialize();
		enemyNormal->SetPosition(position);
		enemyNormal->SetTarget(player_->GetWorldTransform());
	}

	enemies_.push_back(std::move(newEnemy));
}

void GameScene::SpawnEnemyBat(const Vector3& position) {
	std::unique_ptr<BaseEnemy> newEnemy = std::make_unique<SkyTypeEnemy>();
	if (auto* enemyNormal = dynamic_cast<SkyTypeEnemy*>(newEnemy.get())) {
		enemyNormal->Initialize();
		enemyNormal->SetPosition(position);
		enemyNormal->SetTarget(player_->GetWorldTransform());
	}

	enemies_.push_back(std::move(newEnemy));
}

void GameScene::SpawnSet(const Vector3& position) {
	std::unique_ptr<Spawn> newSpawn = std::make_unique<Spawn>();
	newSpawn->Initialize();
	newSpawn->SetPosition(position);
	spawns_.push_back(std::move(newSpawn));
	
}
