#include "TutorialScene.h"

void TutorialScene::Initialize()
{
	//========================================
	// 基底シーン
	BaseScene::Initialize();

	//========================================
	// テクスチャの読み込み

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
	spotLight->direction_ = { 0.0f, -1.0f, 0.0f };
	spotLight->position_ = { 0.0f, 2000.0f, 0.0f };
	spotLight->intensity_ = 3.0f;
	spotLight->decay_ = 1.6f;
	spotLight->distance_ = 3200.0f;
	spotLight->cosAngle_ = 0.2f;
	spotLight->cosFalloffStart_;
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
	player_->SetLockOnSystem(lockOnSystem_.get()); // 🔹 `std::move()` を使わず `get()` でポインタを渡す


	//========================================
	// 当たり判定マネージャ
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(32.0f);

	//========================================
	// フォローカメラのターゲットを設定
	cameraManager_->SetFollowCameraTarget(player_->GetWorldTransform());
	// フォローカメラへ敵のリストの受け渡し
	cameraManager_->GetFollowCamera()->SetEnemiesAndSpawns(&enemies_, &spawns_);

	// アクティブカメラをフォローカメラに設定
	cameraManager_->useFollowCamera_ = true;

	player_->SetFollowCamera(cameraManager_->GetFollowCamera());

	//========================================
	// HUD
	hud_ = std::make_unique<Hud>();
	// 敵とスポーンの情報を最新に保つ
	hud_->SetEnemiesAndSpawns(&enemies_, &spawns_);
	hud_->Initialize(cameraManager_->GetFollowCamera(), player_.get(), lockOnSystem_.get());

	sceneManager_->GetPostEffect()->ApplyEffect("Outline", PostEffect::EffectType::DepthBasedOutline); // 完
}

void TutorialScene::Finalize()
{
	skyDome_.reset();
	ground_.reset();

	player_->Finalize();
}

void TutorialScene::Update()
{
	BaseScene::Update();
	//========================================
	// フェーズ切り替え
	switch (phase_) {
		///=============================================================================
		// フェードイン
	case Phase::kFadeIn:

		if (fade_->IsFinished()) {

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

		///=============================================================================
		// ゲームプレイ
		break;
	case Phase::kPlay:

		//---------------------------------------
		// ゲームのクリア条件
		// クリアの場合
		// TODO: クリア条件を設定
		if (false) {
			isGameClear_ = true;
			isContinue_ = false;
		}
		// ゲームオーバーの場合
		if (player_->GetHp() <= 0) {
			isGameOver_ = true;
			isContinue_ = false;
		}
		// リセット
		if (Input::GetInstance()->Triggerkey(DIK_R)) {
			isContinue_ = false;
		}
		if (!isContinue_) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
		}
		if (isGameClear_) {
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

		// 敵リスト
		for (const auto& enemy : enemies_) {
			enemy->Update();
		}
		{
			std::vector<BaseEnemy*> allEnemies;
			for (const auto& e : enemies_) {
				allEnemies.push_back(e.get());
			}
			for (const auto& s : spawns_) {
				allEnemies.push_back(s.get());
			}
		}
		// 敵の削除
		enemies_.erase(
			// 削除条件
			std::remove_if(enemies_.begin(), enemies_.end(),
				[this](const std::unique_ptr<BaseEnemy>& enemy) {
					// HPが0以下の場合
					if (enemy->GetHp() <= 0) {
						// ロックオンシステムから敵を削除
						if (lockOnSystem_) {
							lockOnSystem_->RemoveLockedEnemy(enemy.get());
						}

						// 削除したエネミーをターゲットに持つプレイヤーのミサイルのターゲットをnullptrに設定
						for (auto it = player_->GetBullets().begin(); it != player_->GetBullets().end(); ++it) {

							if (it->get()->GetTarget() == enemy.get()) {
								it->get()->SetTarget(nullptr);
							}
						}

						return true; // 削除する
					}
					return false; // 削除しない
				}),
			// 実際に削除する
			enemies_.end());

		//---------------------------------------
		// ロックオンの処理追加
		if (lockOnSystem_) {
			// プレイヤーの位置をロックオンシステムにセット
			lockOnSystem_->SetPosition(player_->GetPosition());

			// カメラがFollowCameraの場合、視点方向を設定
			auto activeCamera = cameraManager_->GetActiveCamera();
			if (auto followCamera = dynamic_cast<FollowCamera*>(activeCamera)) {
				// カメラからの視点方向をロックオンシステムに設定
				lockOnSystem_->SetViewDirection(followCamera->GetForwardDirection());
			}
			std::vector<BaseEnemy*> allTargets;
			for (const auto& enemy : enemies_) {
				allTargets.push_back(enemy.get());
			}
			for (const auto& spawn : spawns_) {
				allTargets.push_back(spawn.get());
			}

			lockOnSystem_->DetectEnemiesRaw(allTargets);
			lockOnSystem_->UpdateRaw(allTargets);
			//// 敵の検出
			// lockOnSystem_->DetectEnemies(enemies_);
			// lockOnSystem_->DetectEnemies(spawns_);
			//// ロックオン更新
			// lockOnSystem_->Update(enemies_);
			// lockOnSystem_->Update(spawns_);
		}

		//---------------------------------------
		// 当たり判定
		// リセット
		collisionManager_->Reset();

		// エネミー
		for (auto& enemy : enemies_) {
			collisionManager_->AddCollider(enemy.get());
			// エネミーの弾リスト
			for (auto& bullet : enemy->GetBullets()) {
				collisionManager_->AddCollider(bullet.get());
			}
		}
		// spwan
		for (auto& spawn : spawns_) {
			collisionManager_->AddCollider(spawn.get());
		}
		// プレイヤー
		collisionManager_->AddCollider(player_.get());
		// プレイヤーの弾リスト
		for (auto& bullet : player_->GetBullets()) {
			collisionManager_->AddCollider(bullet.get());
		}
		// プレイヤーのマシンガン弾リスト
		for (auto& machineGunBullet : player_->GetMachineGunBullets()) {
			collisionManager_->AddCollider(machineGunBullet.get());
		}
		// 更新
		collisionManager_->Update();

		//---------------------------------------
		// HUD
		hud_->SetEnemiesAndSpawns(&enemies_, &spawns_);
		hud_->Update();

		//---------------------------------------
		// パーティクル
		ParticleManager::GetInstance()->Update();


		//========================================
		// フェードアウト
		break;
	case Phase::kFadeOut:
		//---------------------------------------
		// `フェードアウトが終わった場合
		if (fade_->IsFinished()) {
			// ゲームをクリアした場合
			if (isGameClear_) {
				// ゲームクリアシーンに遷移
				SceneManager::GetInstance()->ChangeScene("CLEAR");
			}
			else if (isGameOver_) {
				// ゲームオーバーシーンに遷移
				SceneManager::GetInstance()->ChangeScene("OVER");
			}
			else {
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
		// パーティクル
		ParticleManager::GetInstance()->Update();

		break;
	case Phase::kMain:
		break;
	case Phase::kPose:
		break;
	}

	// lineDrawer_->Update();

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

	//

#ifdef _DEBUG

	if (ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("directionalLight.color", &directionalLight->color_.x, ImGuiColorEditFlags_None);
		if (ImGui::DragFloat3("directionalLight.direction", &directionalLight->direction_.x, 0.01f)) {
			directionalLight->direction_ = Normalize(directionalLight->direction_);
		}
		ImGui::DragFloat("directionalLight.intensity", &directionalLight->intensity_, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("pointLight")) {
		ImGui::ColorEdit4("pointLight.color", &pointLight->color_.x, ImGuiColorEditFlags_None);
		ImGui::DragFloat3("pointLight.position", &pointLight->position_.x, 0.01f);
		ImGui::DragFloat("pointLight.decay", &pointLight->decay_, 0.01f);
		ImGui::DragFloat("pointLight.radius", &pointLight->radius_, 0.01f);
		ImGui::DragFloat("pointLight.intensity", &pointLight->intensity_, 0.01f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("spotLight")) {
		ImGui::ColorEdit4("spotlLight.color", &spotLight->color_.x, ImGuiColorEditFlags_None);
		if (ImGui::DragFloat3("spotLight.direction", &spotLight->direction_.x, 0.01f)) {
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

	//ImGui::Checkbox("useDebugCamera", &cameraManager_->useDebugCamera_);
	//ImGui::Checkbox("sceneConticue", &isContinue);

	//hud_->DrawImGui();

#endif

}

void TutorialScene::Draw()
{
	switch (phase_) {
		///=============================================================================
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
		///=============================================================================
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
		for (const auto& enemy : enemies_) {
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
		// if(lockOnSystem_) {
		//	lockOnSystem_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
		//		*directionalLight.get(),
		//		*pointLight.get(),
		//		*spotLight.get());
		//}
		//========================================
		// 当たり判定マネージャ
		collisionManager_->Draw();

		//========================================
		// HUD
		hud_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection());

		DrawForegroundSprite();
		/// 前景スプライト描画

		break;
		///=============================================================================
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
		for (const auto& enemy : enemies_) {
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
		if (lockOnSystem_) {
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
		///=============================================================================
	case Phase::kMain:

		break;
		///=============================================================================
	case Phase::kPose:

		break;
	}

	//========================================
	// パーティクルの描画
	ParticleManager::GetInstance()->Draw("Resources/circle.png");
}
