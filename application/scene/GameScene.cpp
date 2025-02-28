#include "GameScene.h"

#include "imgui.h"

void ShowMatrix4x4(const Matrix4x4& matrix, const char* label) {
	ImGui::Text("%s", label);
	if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders)) {
		// 
		for (int i = 0; i < 4; ++i) {
			ImGui::TableNextRow();
			for (int j = 0; j < 4; ++j) {
				ImGui::TableSetColumnIndex(j);
				ImGui::Text("%.3f", matrix.m[i][j]);
			}
		}
		ImGui::EndTable();
	}
}


void GameScene::Initialize()
{

	BaseScene::Initialize();

	/*TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");*/

	//Audio::GetInstance()->SoundPlayWave("Resources/Alarm01.wav");

	//sprite = std::make_unique<Sprite>();

	//sprite->Initialize(SpriteCommon::GetInstance(), "Resources/uvChecker.png");

	//sprite->SetTexSize({ 512.0f,512.0f });

	ModelManager::GetInstance()->LoadModel("AnimatedCube/AnimatedCube.gltf");

	object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("AnimatedCube/AnimatedCube.gltf");

	object3d->SetCamera(camera.get());

	objectTransform = std::make_unique<WorldTransform>();
	objectTransform->Initialize();

	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();

	animationManager = std::make_unique<AnimationManager>();

	animationManager->LoadAnimationFile("./Resources/AnimatedCube", "AnimatedCube.gltf");

	animationManager->StartAnimation("AnimatedCube.gltf", 0);


	//skyDome
	//ModelManager::GetInstance()->LoadModel("skyDome/skyDome.obj");
	//skyDomeObj_ = std::make_unique<Object3d>();
	//skyDomeObj_->Initialize(Object3dCommon::GetInstance());
	//skyDomeObj_->SetModel("skyDome/skyDome.obj");
	//skyDomeObj_->SetCamera(camera.get());
	//skyDome_->Initialize(std::move(skyDomeObj_));

}

void GameScene::Finalize()
{
}

void GameScene::Update()
{

#ifdef _DEBUG

	if (ImGui::TreeNode("directionalLight")) {
		ImGui::ColorEdit4("directionalLight.color", &directionalLight->color_.x, ImGuiColorEditFlags_None);
		if (ImGui::DragFloat3("directionalLight.direction", &directionalLight->direction_.x, 0.01f))
		{
			directionalLight->direction_ = Normalize(directionalLight->direction_);
		}
		ImGui::DragFloat("directionalLight.intensity", &directionalLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	if (ImGui::TreeNode("pointLight")) {
		ImGui::ColorEdit4("pointLight.color", &pointLight->color_.x, ImGuiColorEditFlags_None);
		ImGui::DragFloat3("pointLight.position", &pointLight->position_.x, 0.01f);
		ImGui::DragFloat("pointLight.decay", &pointLight->decay_, 0.01f);
		ImGui::DragFloat("pointLight.radius", &pointLight->radius_, 0.01f);
		ImGui::DragFloat("pointLight.intensity", &pointLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	if (ImGui::TreeNode("spotLight")) {
		ImGui::ColorEdit4("spotlLight.color", &spotLight->color_.x, ImGuiColorEditFlags_None);
		if (ImGui::DragFloat3("spotLight.direction", &spotLight->direction_.x, 0.01f))
		{
			spotLight->direction_ = Normalize(spotLight->direction_);
		}
		ImGui::DragFloat3("spotLight.position", &spotLight->position_.x, 0.01f);
		ImGui::DragFloat("spotLight.decay", &spotLight->decay_, 0.01f);
		ImGui::DragFloat("spotLight.intensity", &spotLight->intensity_, 0.01f);
		ImGui::TreePop(); // TreeNodeを閉じる
	}

	Matrix4x4 localMatrix = animationManager->GetLocalMatrix();

	ShowMatrix4x4(localMatrix, "localMatrix");

	
	ShowMatrix4x4(objectTransform->matWorld_, "worldMatrix");
#endif

	animationManager->Update();
    objectTransform->UpdateMatrix();
	object3d->Update();
	object3d->SetLocalMatrix(animationManager->GetLocalMatrix());

	if (Input::GetInstance()->Triggerkey(DIK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	pointLight->Update();

	spotLight->Update();


	//skyDome_->Update();
}

void GameScene::Draw()
{

	DrawBackgroundSprite();
	/// 背景スプライト描画


	DrawObject();
	/// オブジェクト描画	


	object3d->Draw(*objectTransform.get(), Camera::GetInstance()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	DrawForegroundSprite();
	/// 前景スプライト描画	
	
}
