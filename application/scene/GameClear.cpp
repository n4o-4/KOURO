#include "GameClear.h"

void GameClear::Initialize() {
	BaseScene::Initialize();

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
	spotLight->position_ = { 0.0f,3200.0f,0.0f };
	spotLight->intensity_ = 11.0f;
	spotLight->decay_ = 0.87f;
	spotLight->distance_ = 4800.0f;
	spotLight->cosAngle_ = 0.2f;
	spotLight->cosFalloffStart_;

	mvSpotLight = std::make_unique<SpotLight>();
	mvSpotLight->Initialize();
	mvSpotLight->direction_ = { 0.0f,-1.0f,0.0f };
	mvSpotLight->position_ = { 0.0f,3.0f,-1.0f };
	mvSpotLight->intensity_ = 11.0f;
	mvSpotLight->decay_ = 0.87f;
	mvSpotLight->distance_ = 4800.0f;
	mvSpotLight->cosAngle_ = 0.2f;
	mvSpotLight->cosFalloffStart_;
	mvSpotLight->color_ = { 0.925f, 0.498f, 0.118f, 1.0f };



	//clear
	TextureManager::GetInstance()->LoadTexture("Resources/scene/game_clear.png");
	clear_ = std::make_unique<Sprite>();
	clear_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/game_clear.png");
	clear_->SetTexSize({ 1280.0f,720.0f });
	clear_->SetSize({ 1280.0f,720.0f });
	clear_->SetPosition({ 0.0f,0.0f });

	AudioManager::GetInstance()->Initialize();
	AudioManager::GetInstance()->SoundLoadFile("Resources/bgm/gameCLEAR.mp3");
	clearBGM_ = std::make_unique<Audio>();
	clearBGM_->Initialize();
	clearBGM_->SoundPlay("Resources/bgm/gameCLEAR.mp3", 9999);

	//model

	ModelManager::GetInstance()->LoadModel("scene/MWbody.obj");
	ModelManager::GetInstance()->LoadModel("scene/MWdoor.obj");
	ModelManager::GetInstance()->LoadModel("scene/dish.obj");
	ModelManager::GetInstance()->LoadModel("scene/Clear.obj");
	ModelManager::GetInstance()->LoadModel("Spawn/Spawn.obj");
	ModelManager::GetInstance()->LoadModel("missile.obj");
	ModelManager::GetInstance()->LoadModel("scene/MWScenen.obj");

	mwbody_ = std::make_unique<Object3d>();
	mwbody_->Initialize(Object3dCommon::GetInstance());
	mwbody_->SetModel("scene/MWbody.obj");

	mwdoor_ = std::make_unique<Object3d>();
	mwdoor_->Initialize(Object3dCommon::GetInstance());
	mwdoor_->SetModel("scene/MWdoor.obj");

	mwdish_ = std::make_unique<Object3d>();
	mwdish_->Initialize(Object3dCommon::GetInstance());
	mwdish_->SetModel("scene/dish.obj");

	clearModel_ = std::make_unique<Object3d>();
	clearModel_->Initialize(Object3dCommon::GetInstance());
	clearModel_->SetModel("scene/Clear.obj");

	spawnModel_ = std::make_unique<Object3d>();
	spawnModel_->Initialize(Object3dCommon::GetInstance());
	spawnModel_->SetModel("Spawn/Spawn.obj");

	missileModel_ = std::make_unique<Object3d>();
	missileModel_->Initialize(Object3dCommon::GetInstance());
	missileModel_->SetModel("missile.obj");

	mvSceneModel_ = std::make_unique<Object3d>();
	mvSceneModel_->Initialize(Object3dCommon::GetInstance());
	mvSceneModel_->SetModel("scene/MWScenen.obj");

	// 初期位置を設定
	mwbodyTransform_ = std::make_unique<WorldTransform>();
	mwbodyTransform_->Initialize();
	mwbodyTransform_->transform.translate = { 0.0f, 1.0f , 1.0f };
	mwbodyTransform_->transform.rotate = { 0.0f, 3.14f, 0.0f };

	mwdoorTransform_ = std::make_unique<WorldTransform>();
	mwdoorTransform_->Initialize();
	mwdoorTransform_->transform.translate = { 1.37f, 0.0f , 0.94f };
	
	mwdishTransform_ = std::make_unique<WorldTransform>();
	mwdishTransform_->Initialize();
	mwdishTransform_->transform.translate = { 0.2f, -0.1f , 0.0f };

	clearTransform_ = std::make_unique<WorldTransform>();
	clearTransform_->Initialize();
	clearTransform_->transform.translate = { 0.9f, 0.6f , 0.2f };
	clearTransform_->transform.scale = { 0.5f, 0.5f , 1.0f };

	spawnTransform_ = std::make_unique<WorldTransform>();
	spawnTransform_->Initialize();
	spawnTransform_->transform.translate = { 23.5f, 10.6f , 38.3f };

	missileTransform_ = std::make_unique<WorldTransform>();
	missileTransform_->Initialize();
	missileTransform_->transform.translate = { 3.6f, 50.7f , 38.3f };
	missileTransform_->transform.rotate = { 0.1f, 1.6f, -1.0f };

	mvSceneTransform_ = std::make_unique<WorldTransform>();
	mvSceneTransform_->Initialize();
	mvSceneTransform_->transform.translate = { 0.0f, 0.0f , 0.0f };


	// パーティクルの初期化
	ParticleManager::GetInstance()->CreateParticleGroup("explosion", "Resources/circle.png", ParticleManager::ParticleType::Normal);
	explosionEmitter_ = std::make_unique<ParticleEmitter>();
	explosionEmitter_->Initialize("explosion");
	// パーティクル設定の調整

	explosionEmitter_->SetParticleCount(100);
	explosionEmitter_->SetFrequency(0.04f);
	explosionEmitter_->SetLifeTimeRange({ 0.5f, 1.0f }); // 寿命を短くする
	explosionEmitter_->SetStartScaleRange(ParticleManager::Vec3Range({ 3.0f,3.0f,3.0f, }, { 5.0f,5.0f,5.0f }));
	explosionEmitter_->SetFinishScaleRange(ParticleManager::Vec3Range({ 3.0f,3.0f,3.0f, }, { 5.0f,5.0f,5.0f }));

	explosionEmitter_->SetVelocityRange(ParticleManager::Vec3Range({ -100.0f,-100.0f,-100.0f }, { 100.0f,100.0f,100.0f }));


	ParticleManager::GetInstance()->GetParticleGroup("explosion")->enableBillboard = true;
	ParticleManager::GetInstance()->GetParticleGroup("explosion")->enablePulse = true;



	mwdoorTransform_->SetParent(mwbodyTransform_.get());
	mwdishTransform_->SetParent(mwbodyTransform_.get());
	clearTransform_->SetParent(mwdishTransform_.get());
	mvSceneTransform_->SetParent(mwdoorTransform_.get());
	



}


void GameClear::Finalize() {
}

void GameClear::Update() {

	BaseScene::Update();
	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();
	mvSpotLight->Update();

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

		//UpdateMoveMwbody();
		//UpdateMoveClear();
		ParticleManager::GetInstance()->Update();

		UpdateMissileFlight();

		mwbodyTransform_->UpdateMatrix();// 行列更新
		mwbody_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		mwbody_->Update();// 更新

		mwdoorTransform_->UpdateMatrix();// 行列更新
		mwdoor_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		mwdoor_->Update();// 更新

		mwdishTransform_->UpdateMatrix();// 行列更新
		mwdish_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		mwdish_->Update();// 更新

		clearTransform_->UpdateMatrix();// 行列更新
		clearModel_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		clearModel_->Update();// 更新

		spawnTransform_->UpdateMatrix();// 行列更新
		spawnModel_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		spawnModel_->Update();// 更新

		missileTransform_->UpdateMatrix();// 行列更新
		missileModel_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		missileModel_->Update();// 更新

		mvSceneTransform_->UpdateMatrix();// 行列更新
		mvSceneModel_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
		mvSceneModel_->Update();// 更新

#ifdef _DEBUG
		ImGui::Begin("Model Transform Editor");

		
		if (ImGui::CollapsingHeader("MW Body")) {
			ImGui::DragFloat3("Position##MWBody", &mwbodyTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##MWBody", &mwbodyTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##MWBody", &mwbodyTransform_->transform.scale.x, 0.01f);
		}


		if (ImGui::CollapsingHeader("MW Door")) {
			ImGui::DragFloat3("Position##MWDoor", &mwdoorTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##MWDoor", &mwdoorTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##MWDoor", &mwdoorTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Dish")) {
			ImGui::DragFloat3("Position##Dish", &mwdishTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##Dish", &mwdishTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##Dish", &mwdishTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Clear")) {
			ImGui::DragFloat3("Position##Clear", &clearTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##Clear", &clearTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##Clear", &clearTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Spawn")) {
			ImGui::DragFloat3("Position##Spawn", &spawnTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##Spawn", &spawnTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##Spawn", &spawnTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("missile")) {
			ImGui::DragFloat3("Position##missile", &missileTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##missile", &missileTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##missile", &missileTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("MW Scene")) {
			ImGui::DragFloat3("Position##MWScenen", &mvSceneTransform_->transform.translate.x, 0.1f);
			ImGui::DragFloat3("Rotation##MWScenen", &mvSceneTransform_->transform.rotate.x, 0.1f);
			ImGui::DragFloat3("Scale##MWScenen", &mvSceneTransform_->transform.scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("mvSpotLight Light")) {
			ImGui::DragFloat3("mvSpotLight Position", &mvSpotLight->position_.x, 1.0f);
			ImGui::DragFloat3("mvSpotLight Direction", &mvSpotLight->direction_.x, 0.05f);
			ImGui::DragFloat("Intensity", &mvSpotLight->intensity_, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Decay", &mvSpotLight->decay_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Distance", &mvSpotLight->distance_, 1.0f, 0.0f, 10000.0f);
			ImGui::DragFloat("CosAngle", &mvSpotLight->cosAngle_, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("FalloffStart", &mvSpotLight->cosFalloffStart_, 0.01f, 0.0f, 1.0f);
			ImGui::ColorEdit3("Color", &mvSpotLight->color_.x);
		}
		ImGui::End();
#endif
		if (Input::GetInstance()->Triggerkey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A))
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

	

	//title
	clear_->Update();

	BaseScene::Update();
}

void GameClear::Draw() {
	DrawBackgroundSprite();

	clear_->Draw();


	DrawObject();

	mwbody_->Draw(*mwbodyTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	mwdoor_->Draw(*mwdoorTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	mwdish_->Draw(*mwdishTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *mvSpotLight);
	clearModel_->Draw(*clearTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *mvSpotLight);
	if (isMissileVisible_) {
	spawnModel_->Draw(*spawnTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	missileModel_->Draw(*missileTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	}
	mvSceneModel_->Draw(*mvSceneTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *mvSpotLight);

	DrawForegroundSprite();

	fade_->Draw();

	ParticleManager::GetInstance()->Draw("Resources/circle.png");
}

void GameClear::UpdateMoveMwbody() {
	if (!isMoving_) return;

	moveTimer_ += 1.0f / 60.0f;
	float t = moveTimer_ / moveDuration_;
	if (t >= 1.0f) {
		t = 1.0f;
		isMoving_ = false;
	}

	// LERP 
	Vector3 startPos = { 0.0f, 1.0f, 1.0f };
	Vector3 endPos = { -0.0f, -0.8f, -11.8f };
	Vector3 newPos = {
		startPos.x + (endPos.x - startPos.x) * t,
		startPos.y + (endPos.y - startPos.y) * t,
		startPos.z + (endPos.z - startPos.z) * t
	};
	mwbodyTransform_->transform.translate = newPos;

	
}

void GameClear::UpdateMissileFlight() {

	if (!isMissileMoving_) return;

	missileTimer_ += 1.0f / 60.0f;
	float t = missileTimer_ / missileDuration_;
	if (t >= 1.0f) {
		t = 1.0f;
		isMissileMoving_ = false;
		isMissileVisible_ = false;

		explosionEmitter_->SetPosition(spawnTransform_->transform.translate);
		explosionEmitter_->Emit();
		
	}


	Vector3 startPos = { 3.6f, 90.7f, 38.3f };
	Vector3 endPos = { 21.4f, 19.5f, 38.3f };
	Vector3 newPos = {
		startPos.x + (endPos.x - startPos.x) * t,
		startPos.y + (endPos.y - startPos.y) * t,
		startPos.z + (endPos.z - startPos.z) * t
	};
	missileTransform_->transform.translate = newPos;

	Vector3 startRot = { 0.1f, 1.6f, -1.0f };
	Vector3 endRot = { 0.3f, 1.6f, -1.0f };
	Vector3 newRot = {
		startRot.x + (endRot.x - startRot.x) * t,
		startRot.y + (endRot.y - startRot.y) * t,
		startRot.z + (endRot.z - startRot.z) * t
	};
	missileTransform_->transform.rotate = newRot;

}

void GameClear::UpdateMoveClear() {
	if (isClear_) {
	mwdishTransform_->transform.rotate.y += 0.05f;
	if (mwdishTransform_->transform.rotate.y >= 12.65) {
		mwdishTransform_->transform.rotate.y = 12.65f;
		isClear_ = false;
		}
	}
	if (!isClear_) {
		if (isDoor_) {
		doorTimer_ += 1.0f / 60.0f;
		float t = doorTimer_ / doorDuration_;

		Vector3 startRot = { 0.0f, 0.0f, 0.0f };
		Vector3 endRot = { 0.0f, 3.0f, 0.0f };
		Vector3 newRot = {
			startRot.x + (endRot.x - startRot.x) * t,
			startRot.y + (endRot.y - startRot.y) * t,
			startRot.z + (endRot.z - startRot.z) * t
			};
		mwdoorTransform_->transform.rotate = newRot;
		}
		if (doorTimer_ >= doorDuration_) {
			isDoor_ = false;
		}
	}
}

