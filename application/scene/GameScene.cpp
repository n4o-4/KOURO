#include "GameScene.h"
#include "imgui.h"
#include <imgui_internal.h>

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

//void ShowColorRangeEditor(const char* label) {
//
//	ParticleManager::ParticleGroup* group = ParticleManager::GetInstance()->GetParticleGroup(label);
//
//	if (ImGui::TreeNode(label)) {
//
//		ImGui::Text("startColor Range");
//
//		Vector4 startColor
//
//		ImGui::ColorEdit4("Min", (float*)&group->startColorRange.R.min, ImGuiColorEditFlags_Float);
//
//		// グラデーションの可視化：左=Min、右=Max
//		ImVec4 colorMin = ImVec4(colorRange.R.min, colorRange.G.min, colorRange.B.min, colorRange.A.min);
//		ImVec4 colorMax = ImVec4(colorRange.R.max, colorRange.G.max, colorRange.B.max, colorRange.A.max);
//
//
//
//		ImGui::Text("Gradient Preview:");
//		ImDrawList* drawList = ImGui::GetWindowDrawList();
//		ImVec2 p = ImGui::GetCursorScreenPos();
//		float width = ImGui::GetContentRegionAvail().x;
//		float height = 20.0f;
//		ImVec2 p1 = ImVec2(p.x + width, p.y + height);
//
//		// グラデーション矩形を描画
//		drawList->AddRectFilledMultiColor(p, p1,
//			ImColor(colorMin), ImColor(colorMax),
//			ImColor(colorMax), ImColor(colorMin));
//
//		ImGui::Dummy(ImVec2(width, height)); // スペースを確保
//
//		ImGui::TreePop();
//	}
//}

///=============================================================================
///						初期化
void GameScene::Initialize() {
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
	spotLight->direction_ = { 0.0f,-1.0f,0.0f };
	spotLight->position_ = { 0.0f,10.0f,0.0f };
	spotLight->intensity_ = 1.0f;
	spotLight->decay_ = 0.87f;
	spotLight->distance_ = 40.0f;
	spotLight->cosAngle_ = 0.5f;

	//========================================
	// アクティブカメラをフォローカメラに設定
	cameraManager_->useDebugCamera_ = true;

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

    //dissolve_ = dynamic_cast<Dissolve*>(sceneManager_->GetPostEffect()->GetEffectData("dissolve"));
	colorSpace_ = dynamic_cast<ColorSpace*>(sceneManager_->GetPostEffect()->GetEffectData("colorSpace"));

	///========================================
	///		ライン描画
	lineDrawer_ = std::make_unique<LineDrawerBase>();
	lineDrawer_->Initialize(sceneManager_->GetDxCommon(),sceneManager_->GetSrvManager());

	lineDrawer_->CreateLineObject(LineDrawerBase::Type::Grid, nullptr);

	ModelManager::GetInstance()->LoadModel("terrain.obj");

	ground_ = std::make_unique<Object3d>();
	ground_->Initialize(Object3dCommon::GetInstance());
	ground_->SetModel(ModelManager::GetInstance()->FindModel("terrain.obj"));
	ground_->SetLocalMatrix(MakeIdentity4x4());

	transform_ = std::make_unique<WorldTransform>();
	transform_->Initialize();

	///========================================
	///		パーティクル

	ParticleManager::GetInstance()->SetBlendMode("Add");

	// plane
	ParticleManager::GetInstance()->CreateParticleGroup("plane_Particle", "Resources/circle.png", ParticleManager::ParticleType::Normal);

	// billboardを有効
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enableBillboard = true;

	// 減速を有効
	ParticleManager::GetInstance()->GetParticleGroup("plane_Particle")->enableDeceleration = true;

	emitter1_ = std::make_unique<ParticleEmitter>();
	emitter1_->Initialize("plane_Particle");
	emitter1_->SetPosition({ 0.0f,2.0f,0.0f });
	emitter1_->SetParticleCount(100);
	emitter1_->SetVelocityRange({ {-10.0f,10.0f},{-10.0f,10.0f},{-10.0f,10.0f} });

	emitter1_->SetScaleRange({ {0.3f,0.3f},{0.3f,0.3f},{0.3f,0.3f} });
	
	emitter1_->SetStartColorRange({  {0.56f,1.0f},{0.0f,0.37f},{0.0f,0.19f},{1.0f,1.0f} });
	emitter1_->SetFinishColorRange({ {0.56f,1.0f},{0.0f,0.37f},{0.0f,0.19f},{1.0f,1.0f} });
	
	emitter1_->SetLifeTimeRange({ 0.5f,0.8f });
	emitter1_->SetFrequency(1.0f);



	// ring1

	ParticleManager::GetInstance()->CreateParticleGroup("ring1_Particle", "Resources/gradationLine.png",ParticleManager::ParticleType::Ring);

	ParticleManager::GetInstance()->GetParticleGroup("ring1_Particle")->enableBillboard = true;
	

	emitter2_ = std::make_unique<ParticleEmitter>();
	emitter2_->Initialize("ring1_Particle");
	emitter2_->SetPosition({ 0.0f,2.0f,0.0f });
	emitter2_->SetParticleCount(1);
	emitter2_->SetVelocityRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} });
	emitter2_->SetStartColorRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f} });
	emitter2_->SetFinishColorRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} });
	emitter2_->SetLifeTimeRange({ 1.0f,1.0f });
	emitter2_->SetFrequency(1.5f);
	
	// ring2
	
	ParticleManager::GetInstance()->CreateParticleGroup("ring2_Particle", "Resources/gradationLine.png", ParticleManager::ParticleType::Ring);

	emitter3_ = std::make_unique<ParticleEmitter>();
	emitter3_->Initialize("ring2_Particle");
	emitter3_->SetPosition({ 0.0f,2.0f,0.0f });
	emitter3_->SetParticleCount(3);
	emitter3_->SetVelocityRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} });
	emitter3_->SetStartColorRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f} });
	emitter3_->SetFinishColorRange({ {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} });
	emitter3_->SetLifeTimeRange({ 1.0f,1.0f });
	emitter3_->SetFrequency(1.5f);

	///========================================
	/// 入力

	// マウスの入力受付を有効
	Input::GetInstance()->SetIsReception(true);


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
	/*animationManager->Update();
	lineDrawer_->SkeletonUpdate(animationManager->GetActiveAnimation("walk.gltf").skeleton);*/
	//human_->Update();
	
	//lineDrawer_->Update();

	emitter1_->Update();

	BaseScene::Update();

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

	
	if (ImGui::Button("Add Particle plane and ring"))
	{
		emitter1_->Emit();
		//emitter2_->Emit();
	}

	if (ImGui::Button("Add Particle plane"))
	{
		emitter3_->HitEmit();
	}

#endif

	lineDrawer_->Update();
}

///=============================================================================
///						描画
void GameScene::Draw() 
{

	DrawBackgroundSprite();
	/// 背景スプライト描画

	DrawObject();
	/// オブジェクト描画

	ground_->Draw(*transform_.get(), cameraManager_->GetActiveCamera()->GetViewProjection(), *directionalLight.get(), *pointLight.get(), *spotLight.get());

	DrawForegroundSprite();
	/// 前景スプライト描画

	// フェード描画
	DrawFade();

	//========================================
	//パーティクルの描画
	ParticleManager::GetInstance()->Draw("Resources/circle.png");	

	//lineDrawer_->Draw(cameraManager_->GetActiveCamera()->GetViewProjection());
}