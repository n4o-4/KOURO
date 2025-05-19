#include "TitleScene.h"


void TitleScene::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");


	ModelManager::GetInstance()->LoadModel("axis.obj");

	//audio = std::make_unique<Audio>();
	//audio->Initialize();
	//audio->SoundPlay("Resources/Spinning_World.mp3",999);

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

	//title
	TextureManager::GetInstance()->LoadTexture("Resources/scene/TiN.png");
	title_ = std::make_unique<Sprite>();
	title_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/TiN.png");
	title_->SetTexSize({ 1280.0f,720.0f });
	title_->SetSize({ 1280.0f,720.0f });
	title_->SetPosition({ 0.0f,0.0f });
	//start
	TextureManager::GetInstance()->LoadTexture("Resources/scene/A1.png");
	start1_ = std::make_unique<Sprite>();
	start1_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/A1.png");
	start1_->SetTexSize({ 220.0f,220.0f });
	start1_->SetSize({ 220.0f,220.0f });
	start1_->SetPosition({ 235.0f,330.0f });
	TextureManager::GetInstance()->LoadTexture("Resources/scene/A2.png");
	start2_ = std::make_unique<Sprite>();
	start2_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/A2.png");
	start2_->SetTexSize({ 230.0f,230.0f });
	start2_->SetSize({ 230.0f,230.0f });
	start2_->SetPosition({ 230.0f,325.0f });

	//select
	TextureManager::GetInstance()->LoadTexture("Resources/scene/select1.png");
	select1_ = std::make_unique<Sprite>();
	select1_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/select1.png");
	select1_->SetTexSize({ 240.0f,90.0f });
	select1_->SetSize({ 270.0f,120.0f });
	select1_->SetPosition({ 728.0f,90.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/select2.png");
	select2_ = std::make_unique<Sprite>();
	select2_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/select2.png");
	select2_->SetTexSize({ 240.0f,90.0f });
	select2_->SetSize({ 240.0f,90.0f });
	select2_->SetPosition({ 728.0f,240.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/select3.png");
	select3_ = std::make_unique<Sprite>();
	select3_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/select3.png");
	select3_->SetTexSize({ 240.0f,90.0f });
	select3_->SetSize({ 240.0f,90.0f });
	select3_->SetPosition({ 728.0f,390.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/select4.png");
	select4_ = std::make_unique<Sprite>();
	select4_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/select4.png");
	select4_->SetTexSize({ 240.0f,90.0f });
	select4_->SetSize({ 240.0f,90.0f });
	select4_->SetPosition({ 728.0f,540.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/operation.png");
	operation_ = std::make_unique<Sprite>();
	operation_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/operation.png");
	operation_->SetTexSize({ 1280.0f,720.0f });
	operation_->SetSize({ 1280.0f,720.0f });
	operation_->SetPosition({ 0.0f,0.0f });
	// 天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize();

	ModelManager::GetInstance()->LoadModel("player/Microwave.obj");
	player_ = std::make_unique<Object3d>();
	player_->Initialize(Object3dCommon::GetInstance());
	player_->SetModel("player/Microwave.obj");
	
	playerWorldTransform_ = std::make_unique<WorldTransform>();
	playerWorldTransform_->Initialize();
	playerWorldTransform_->transform.scale = { 2.0f,2.0f,2.0f };
	playerWorldTransform_->transform.rotate = { 0.0f,3.7f,0.0f };
	playerWorldTransform_->transform.translate = { 5.0f,-2.8f,0.0f };

	//
	start = false;
	operation = false;
	easy = false;
	nomal = false;
	hard = false;
	stsrtTime = 50;

	AudioManager::GetInstance()->Initialize();
	AudioManager::GetInstance()->SoundLoadFile("Resources/bgm/title.mp3");
	AudioManager::GetInstance()->SoundLoadFile("Resources/se/47.mp3");

	bgm_ = std::make_unique<Audio>();
	se1_ = std::make_unique<Audio>();

	bgm_->Initialize();
	se1_->Initialize();

	bgm_->SoundPlay("Resources/bgm/title.mp3", 10000);
}

void TitleScene::Finalize() {
	BaseScene::Finalize();

	bgm_->SoundStop("Resources/bgm/title.mp3");
}

void TitleScene::Update() {
	switch (phase_) {
	case Phase::kFadeIn:

		if (fade_->IsFinished()) {
			Input::GetInstance()->SetIsReception(true);
			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:
		
			if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
				if ((easy || nomal || hard) && selectNum != 3) {

					fade_->Start(Fade::Status::FadeOut, fadeTime_);
					phase_ = Phase::kFadeOut;
				}
			}
		
		

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished()) {
			SceneManager::GetInstance()->ChangeScene("GAME");

			return;
		}

		break;

	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	BaseScene::Update();
	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();

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
	if (ImGui::TreeNode("condition")) {
		ImGui::TextWrapped("stsrtTime : %d", stsrtTime);
		Vector2 pos1 = select1_->GetPosition(); 
		Vector2 pos2 = select2_->GetPosition();
		Vector2 pos3 = select3_->GetPosition();
		float posArr1[2] = { pos1.x, pos1.y };
		float posArr2[2] = { pos2.x, pos2.y };
		float posArr3[2] = { pos3.x, pos3.y };
		if (ImGui::DragFloat2("select1_.position", posArr1, 1.0f)) {
			select1_->SetPosition(Vector2(posArr1[0], posArr1[1]));
		}
		if (ImGui::DragFloat2("select2_.position", posArr2, 1.0f)) {
			select2_->SetPosition(Vector2(posArr2[0], posArr2[1]));
		}
		if (ImGui::DragFloat2("select3_.position", posArr3, 1.0f)) {
			select3_->SetPosition(Vector2(posArr3[0], posArr3[1]));
		}
		ImGui::TreePop();

		if (ImGui::Checkbox("flag", &music))
		{
			music = false;

			//audio->SoundPlay("Resources/Spinning_World.mp3", 1);
		}
	}
#endif

	skyDome_->Update();

	playerWorldTransform_->UpdateMatrix();
	player_->SetLocalMatrix(MakeIdentity4x4());
	player_->Update();

	//title
	title_->Update();
	start1_->Update();
	start2_->Update();
	//select
	select1_->Update();
	select2_->Update();
	select3_->Update();
	select4_->Update();
	operation_->Update();
	if (!start) {
		if(Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)){
			start = true;
		}
	}
	if (start) {
	select();
	
	}

}

void TitleScene::Draw()
{
	BaseScene::Draw();

	DrawBackgroundSprite();
	/// 背景スプライト描画


	DrawObject();
	/// オブジェクト描画	

	skyDome_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

	player_->Draw(*playerWorldTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(), 
		*directionalLight, *pointLight, *spotLight);

	DrawForegroundSprite();	
	/// 前景スプライト描画	
	title_->Draw();
	if (!start) {
		stsrtTime--;
		if (stsrtTime <= 50) {
			start1_->Draw();
			if (stsrtTime <= 25) {
				start2_->Draw();
			}
		}
		if (stsrtTime == 0) {
			stsrtTime = 50;
		}
	}
	if (start) {
		select1_->Draw();
		select2_->Draw();
		select3_->Draw();
		select4_->Draw();
	}
	if (operation) {
		operation_->Draw();
	}
	
	fade_->Draw();

}

void TitleScene::select() {

	playerWorldTransform_->transform.rotate = { 0.0f,-3.7f,0.0f };
	playerWorldTransform_->transform.translate = { -5.0f,-2.8f,0.0f };
	title_->SetPosition({ -200.0f,0.0f });

	if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::DPAD_DOWN)) {
		selectNum++;
	}
	if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::DPAD_UP)) {
		selectNum--;
	}
	if (selectNum > 4) {
		selectNum = 3;
	}
	if (selectNum < -1) {
		selectNum = 0;
	}
	if (selectNum == 0) {
		select1_->SetSize({ 270.0f,120.0f });
		select2_->SetSize({ 240.0f,90.0f });
		select3_->SetSize({ 240.0f,90.0f });
		select4_->SetSize({ 240.0f,90.0f });
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			easy = true;
			SceneManager::GetInstance()->GetTransitionData().easy = true;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
	}
	if (selectNum == 1) {
		select2_->SetSize({ 270.0f,120.0f });
		select3_->SetSize({ 240.0f,90.0f });
		select4_->SetSize({ 240.0f,90.0f });
		select1_->SetSize({ 240.0f,90.0f });
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			nomal = true;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = true;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
	}
	if (selectNum == 2) {
		select3_->SetSize({ 270.0f,120.0f });
		select4_->SetSize({ 240.0f,90.0f });
		select1_->SetSize({ 240.0f,90.0f });
		select2_->SetSize({ 240.0f,90.0f });
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			hard = true;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = true;
		}
	}
	if (selectNum == 3) {
		select4_->SetSize({ 270.0f,120.0f });
		select1_->SetSize({ 240.0f,90.0f });
		select2_->SetSize({ 240.0f,90.0f });
		select3_->SetSize({ 240.0f,90.0f });
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			operation = !operation;
			easy = false;
			nomal = false;
			hard = false;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
		
	}
}
