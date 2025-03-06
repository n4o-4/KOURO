#include "GameScene.h"
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

	// 
	fade_->Start(Fade::Status::FadeIn, 4.0f);
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
	switch(phase_) {
	case Phase::kFadeIn:

		if(fade_->IsFinished()) {

			phase_ = Phase::kPlay;

			// 入力の受付をオンに
			Input::GetInstance()->SetIsReception(true);
		}

		//========================================
		// プレイヤーの更新
		player_->Update();

		//========================================
		// 天球
		skyDome_->Update();

		//========================================
		// 地面
		ground_->Update();

		break;

	case Phase::kPlay:

		if(!isContinue) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
		}

		//========================================
		// プレイヤーの更新
		player_->Update();

		//========================================
		// 天球
		skyDome_->Update();

		//========================================
		// 地面
		ground_->Update();

		//========================================
		// 敵出現
		UpdateEnemyPopCommands();
		// 敵リスト
		for(const auto &enemy : enemies_) {
			enemy->Update();
		}
		// 敵の削除
		enemies_.erase(
			// 削除条件
			std::remove_if(enemies_.begin(), enemies_.end(),
				[this](const std::unique_ptr<Enemy> &enemy) {
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

		//========================================
		// 当たり判定
		// リセット
		collisionManager_->Reset();

		// 🔽 ロックオンの処理追加
		if(lockOnSystem_) {
			// プレイヤーの位置をロックオンシステムにセット
			lockOnSystem_->SetPosition(player_->GetPosition());

			// カメラがFollowCameraの場合、視点方向を設定
			auto activeCamera = cameraManager_->GetActiveCamera();
			if(auto followCamera = dynamic_cast<FollowCamera *>( activeCamera )) {
				// カメラからの視点方向をロックオンシステムに設定
				lockOnSystem_->SetViewDirection(followCamera->GetForwardDirection());
			}

			// ロックオン更新
			lockOnSystem_->Update(enemies_);
			// 敵の検出
			lockOnSystem_->DetectEnemies(enemies_);
		}

		// エネミー
		for(auto &enemy : enemies_) {
			collisionManager_->AddCollider(enemy.get());
		}

		// プレイヤー
		collisionManager_->AddCollider(player_.get());

		// プレイヤーの弾リスト
		for(auto &bullet : player_->GetBullets()) {
			collisionManager_->AddCollider(bullet.get());
		}

		// 更新
		collisionManager_->Update();

		break;

	case Phase::kFadeOut:

		if(fade_->IsFinished()) {
			sceneManager_->ChangeScene("CLEAR");
		}

		//========================================
		// プレイヤーの更新
		player_->Update();

		//========================================
		// 天球
		skyDome_->Update();

		//========================================
		// 地面
		ground_->Update();

		break;

	case Phase::kMain:
		break;
	case Phase::kPose:
		break;
	}


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
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
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
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
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

	case Phase::kFadeOut:

		DrawBackgroundSprite();
		/// 背景スプライト描画

		DrawObject();
		/// オブジェクト描画
		//========================================
		// プレイヤーの描画
		player_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
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
}

///=============================================================================
///                        静的メンバ関数
///--------------------------------------------------------------
///                        敵の出現データの読み込み
void GameScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
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

			SpawnEnemy(Vector3(x, y, z));
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

	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize();
	newEnemy->SetPosition(position);
	//newEnemy->SetGameScene(this);
	//newEnemy->SetPlayer(player_);
	enemies_.push_back(std::move(newEnemy));

}
