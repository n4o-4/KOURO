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
	TextureManager::GetInstance()->LoadTexture("Resources/scene/Tutorial.png");
	select1_ = std::make_unique<Sprite>();
	select1_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/Tutorial.png");
	select1_->SetTexSize({ 110.0f,81.0f });
	select1_->SetSize({ 110.0f,81.0f });
	select1_->SetPosition({ 844.0f,190.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/Easy.png");
	select2_ = std::make_unique<Sprite>();
	select2_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/Easy.png");
	select2_->SetTexSize({ 110.0f,81.0f });
	select2_->SetSize({ 110.0f,81.0f });
	select2_->SetPosition({ 844.0f,190.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/Nomal.png");
	select3_ = std::make_unique<Sprite>();
	select3_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/Nomal.png");
	select3_->SetTexSize({ 110.0f,81.0f });
	select3_->SetSize({ 110.0f,81.0f });
	select3_->SetPosition({ 844.0f,190.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/Hard.png");
	select4_ = std::make_unique<Sprite>();
	select4_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/Hard.png");
	select4_->SetTexSize({ 110.0f,81.0f });
	select4_->SetSize({ 110.0f,81.0f });
	select4_->SetPosition({ 844.0f,190.0f });

	TextureManager::GetInstance()->LoadTexture("Resources/scene/operation.png");
	operation_ = std::make_unique<Sprite>();
	operation_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/operation.png");
	operation_->SetTexSize({ 1280.0f,720.0f });
	operation_->SetSize({ 1280.0f,720.0f });
	operation_->SetPosition({ 0.0f,0.0f });
	// 天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize();

	//model
	ModelManager::GetInstance()->LoadModel("titleM/wv.obj");
	ModelManager::GetInstance()->LoadModel("titleM/wvb.obj");
	ModelManager::GetInstance()->LoadModel("titleM/wvr.obj");
	mv_ = std::make_unique<Object3d>();
	mv_->Initialize(Object3dCommon::GetInstance());
	mv_->SetModel("titleM/wv.obj");
	
	mvWorldTransform_ = std::make_unique<WorldTransform>();
	mvWorldTransform_->Initialize();
	mvWorldTransform_->transform.scale = { 4.9f,4.9f,4.9f };
	mvWorldTransform_->transform.rotate = { -0.08f,3.14f,0.0f };
	mvWorldTransform_->transform.translate = { 0.0f,-8.2f,3.0f };

	mvB_ = std::make_unique<Object3d>();
	mvB_->Initialize(Object3dCommon::GetInstance());
	mvB_->SetModel("titleM/wvb.obj");

	mvBWorldTransform_ = std::make_unique<WorldTransform>();
	mvBWorldTransform_->Initialize();
	mvBWorldTransform_->transform.scale = { 4.9f,4.9f,4.9f };
	mvBWorldTransform_->transform.rotate = { -0.08f,3.14f,0.0f };
	mvBWorldTransform_->transform.translate = { 0.0f,-8.2f,3.0f };

	mvR_ = std::make_unique<Object3d>();
	mvR_->Initialize(Object3dCommon::GetInstance());
	mvR_->SetModel("titleM/wvr.obj");

	mvRWorldTransform_ = std::make_unique<WorldTransform>();
	mvRWorldTransform_->Initialize();
	mvRWorldTransform_->transform.scale = { 4.9f,4.9f,4.9f };
	mvRWorldTransform_->transform.rotate = { -0.08f,3.14f,0.0f };
	mvRWorldTransform_->transform.translate = { 4.4f,-1.9f,-1.8f };

	
	easy = false;
	nomal = false;
	hard = false;

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
		ImGui::Text("selectNum: %d", selectNum);
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

		if (ImGui::TreeNode("model")) {
			if (ImGui::TreeNode("mv_")) {
				ImGui::DragFloat3("Scale", &mvWorldTransform_->transform.scale.x, 0.1f);
				ImGui::DragFloat3("Rotate", &mvWorldTransform_->transform.rotate.x, 0.01f);
				ImGui::DragFloat3("Translate", &mvWorldTransform_->transform.translate.x, 0.1f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("mvB_")) {
				ImGui::DragFloat3("Scale##B", &mvBWorldTransform_->transform.scale.x, 0.1f);
				ImGui::DragFloat3("Rotate##B", &mvBWorldTransform_->transform.rotate.x, 0.01f);
				ImGui::DragFloat3("Translate##B", &mvBWorldTransform_->transform.translate.x, 0.1f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("mvR_")) {
				ImGui::DragFloat3("Scale##R", &mvRWorldTransform_->transform.scale.x, 0.1f);
				ImGui::DragFloat3("Rotate##R", &mvRWorldTransform_->transform.rotate.x, 0.01f);
				ImGui::DragFloat3("Translate##R", &mvRWorldTransform_->transform.translate.x, 0.1f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}


	}
#endif

	skyDome_->Update();

	mvWorldTransform_->UpdateMatrix();
	mv_->SetLocalMatrix(MakeIdentity4x4());
	mv_->Update();
	mvBWorldTransform_->UpdateMatrix();
	mvB_->SetLocalMatrix(MakeIdentity4x4());
	mvB_->Update();
	mvRWorldTransform_->UpdateMatrix();
	mvR_->SetLocalMatrix(MakeIdentity4x4());
	mvR_->Update();

	//title
	title_->Update();
	//select
	select();
	select1_->Update();
	select2_->Update();
	select3_->Update();
	select4_->Update();
	
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

	mv_->Draw(*mvWorldTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	mvB_->Draw(*mvBWorldTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);
	mvR_->Draw(*mvRWorldTransform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(),
		*directionalLight, *pointLight, *spotLight);

	DrawForegroundSprite();	
	/// 前景スプライト描画	

	title_->Draw();
	if (selectNum == 0) {
		select1_->Draw();
	} else if (selectNum == 1) {
		select2_->Draw();
	} else if (selectNum == 2) {
		select3_->Draw();
	} else if (selectNum == 3) {
		select4_->Draw();
	}  
		
	
	
	
	if (operation) {
		operation_->Draw();
	}
	
	fade_->Draw();

}

void TitleScene::select() {

	//title_->SetPosition({ -200.0f,0.0f });

	Vector2 stickInput = Input::GetInstance()->GetLeftStick();

		if (stickInput.x > 0.5f && stickReleased_) {
			selectNum++;
			stickReleased_ = false;
			mvRWorldTransform_->transform.rotate.z -= 0.4f;
		}

		else if (stickInput.x < -0.5f && stickReleased_) {
			selectNum--;
			stickReleased_ = false;
			mvRWorldTransform_->transform.rotate.z += 0.4f;
		}

		if (std::abs(stickInput.x) < 0.3f) {
			stickReleased_ = true;
		}
	//}
	if (selectNum >= 3) {
		selectNum = 3;
	}
	if (selectNum <= 0) {
		selectNum = 0;
	}
	if (selectNum == 0) {
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			mvBWorldTransform_->transform.translate.z += 0.2f;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
	}
	if (selectNum == 1) {
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			mvBWorldTransform_->transform.translate.z += 0.2f;
			easy = true;
			SceneManager::GetInstance()->GetTransitionData().easy = true;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
	}
	if (selectNum == 2) {
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			mvBWorldTransform_->transform.translate.z += 0.2f;
			nomal = true;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = true;
			SceneManager::GetInstance()->GetTransitionData().hard = false;
		}
	}
	if (selectNum == 3) {
		if (Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			se1_->SoundPlay("Resources/se/47.mp3", 0);
			mvBWorldTransform_->transform.translate.z += 0.2f;
			hard = true;
			SceneManager::GetInstance()->GetTransitionData().easy = false;
			SceneManager::GetInstance()->GetTransitionData().nomal = false;
			SceneManager::GetInstance()->GetTransitionData().hard = true;
		}
		
	}
}
