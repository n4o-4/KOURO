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

std::vector<ParticleManager::GradationPoint> ConvertToGradationPoints(const std::vector<ParticleManager::GradationPoint>& gradientPoints) {
	std::vector<ParticleManager::GradationPoint> gradationPoints;
	gradationPoints.reserve(gradientPoints.size());
	for (const auto& point : gradientPoints) {
		ParticleManager::GradationPoint gradationPoint;
		gradationPoint.ration = point.ration;
		gradationPoint.color = { point.color.x, point.color.y, point.color.z, point.color.w };
		gradationPoints.push_back(gradationPoint);
	}
	return gradationPoints;
}

ImVec4 ConvertToImVec4(const Vector4& vec) {
	return ImVec4(vec.x, vec.y, vec.z, vec.w);
}

void ShowGradientEditor(std::string name) {
    std::vector<ParticleManager::GradationPoint>* gradationPoints =  &ParticleManager::GetInstance()->GetParticleGroup(name)->gradationPoints;
	ImGui::Text("Gradient Editor");

	//// 制御点の編集
	//for (size_t i = 0; i < gradationPoints->size(); ++i) {
	//	
	//	ImGui::DragFloat("Position", *gradationPoints[i]->ration, 0.01f, 0.0f, 1.0f);
	//	ImGui::ColorEdit4("Color", (float*)&gradationPoints[i].color);

	//	// 削除ボタン
	//	if (ImGui::Button("Remove")) {
	//		ImGui::PopID(); // ★ PushID の対応を忘れずに
	//		gradationPoints->erase(gradationPoints->begin() + i);
	//		break;
	//	}

	//	ImGui::PopID(); // ★ 通常パスも忘れずに
	//}

	// 修正箇所: gradationPoints[i] をポインタではなく参照として扱うように変更  
	for (size_t i = 0; i < gradationPoints->size(); ++i) {
		ImGui::PushID(static_cast<int>(i));

		// 修正: ポインタ演算子 -> を参照演算子 . に変更  
		ImGui::DragFloat("Position", &(*gradationPoints)[i].ration, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit4("Color", (float*)&(*gradationPoints)[i].color);

		// 削除ボタン  
		if (ImGui::Button("Remove")) {
			ImGui::PopID(); // ★ PushID の対応を忘れずに  
			gradationPoints->erase(gradationPoints->begin() + i);
			break;
		}

		ImGui::PopID(); // ★ 通常パスも忘れずに  
	}

	// グラデーションのプレビュー
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 canvasPos = ImGui::GetCursorScreenPos();
	ImVec2 canvasSize = ImVec2(300, 50); // プレビューのサイズ
	drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255));

	/*for (size_t i = 0; i + 1 < gradationPoints->size(); ++i) {
       
		ImVec2 start = ImVec2(canvasPos.x + gradationPoints[i]->ration * canvasSize.x, canvasPos.y);
		ImVec2 end = ImVec2(canvasPos.x + gradationPoints[i + 1].ration * canvasSize.x, canvasPos.y + canvasSize.y);
		ImU32 colStart = ImGui::ColorConvertFloat4ToU32(ConvertToImVec4(gradationPoints[i].color));
		ImU32 colEnd = ImGui::ColorConvertFloat4ToU32(ConvertToImVec4(gradationPoints[i + 1].color));
		drawList->AddRectFilledMultiColor(start, end, colStart, colEnd, colEnd, colStart);
	}*/

	// 修正箇所: gradationPoints[i] をポインタではなく参照として扱うように変更  
	for (size_t i = 0; i + 1 < gradationPoints->size(); ++i) {
		ImVec2 start = ImVec2(canvasPos.x + (*gradationPoints)[i].ration * canvasSize.x, canvasPos.y);
		ImVec2 end = ImVec2(canvasPos.x + (*gradationPoints)[i + 1].ration * canvasSize.x, canvasPos.y + canvasSize.y);
		ImU32 colStart = ImGui::ColorConvertFloat4ToU32(ConvertToImVec4((*gradationPoints)[i].color));
		ImU32 colEnd = ImGui::ColorConvertFloat4ToU32(ConvertToImVec4((*gradationPoints)[i + 1].color));
		drawList->AddRectFilledMultiColor(start, end, colStart, colEnd, colEnd, colStart);
	}

	ImGui::Dummy(canvasSize); // スペースを確保

	//// コントロールポイント追加ボタン
	//if (ImGui::Button("Add Control Point")) {
	//	ParticleManager::GradationPoint newPoint;
	//	newPoint.ration = 0.5f;
	//	newPoint.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//	gradationPoints.push_back(newPoint);
	//	std::sort(gradationPoints.begin(), gradationPoints.end(), [](const ParticleManager::GradationPoint& a, const ParticleManager::GradationPoint& b) {
	//		return a.ration < b.ration;
	//		});
	//}

	if (ImGui::Button("Add Control Point")) {
		ParticleManager::GradationPoint newPoint;
		newPoint.ration = 0.5f;
		newPoint.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		gradationPoints->push_back(newPoint);
		std::sort(gradationPoints->begin(), gradationPoints->end(), [](const ParticleManager::GradationPoint& a, const ParticleManager::GradationPoint& b) {
			return a.ration < b.ration;
			});
	}

	// コントロールポイントのドラッグ処理
	const float knobRadius = 5.0f;
	static int draggingIndex = -1;

	for (size_t i = 0; i < gradationPoints->size(); ++i) {
		ImGui::PushID(static_cast<int>(i));

		float x = canvasPos.x + (*gradationPoints)[i].ration * canvasSize.x;
		float y = canvasPos.y + canvasSize.y + 10.0f;
		ImVec2 knobCenter = ImVec2(x, y);
		drawList->AddCircleFilled(knobCenter, knobRadius, ImGui::ColorConvertFloat4ToU32(ConvertToImVec4((*gradationPoints)[i].color)));

		ImRect knobRect(ImVec2(x - knobRadius, y - knobRadius), ImVec2(x + knobRadius, y + knobRadius));
		if (draggingIndex == -1 && ImGui::IsMouseHoveringRect(knobRect.Min, knobRect.Max) && ImGui::IsMouseClicked(0)) {
			draggingIndex = static_cast<int>(i);
		}

		ImGui::PopID();
	}

	if (draggingIndex != -1 && ImGui::IsMouseDown(0)) {
		float mouseX = ImGui::GetIO().MousePos.x;
		float newPos = (mouseX - canvasPos.x) / canvasSize.x;
		newPos = std::clamp(newPos, 0.0f, 1.0f);
		(*gradationPoints)[draggingIndex].ration = newPos;

		std::sort(gradationPoints->begin(), gradationPoints->end(), [](const ParticleManager::GradationPoint& a, const ParticleManager::GradationPoint& b) {
			return a.ration < b.ration;
			});
	}
	else if (draggingIndex != -1 && !ImGui::IsMouseDown(0)) {
		draggingIndex = -1; // ドラッグ終了
        // Add a conversion function to convert GradientPoint to ParticleManager::GradationPoint
 
        // 修正箇所: gradationPoints をポインタから参照に変換して渡す  
        //ParticleManager::GetInstance()->SetParticleGropGradationPoints("name", ConvertToGradationPoints(*gradationPoints));
	}
}

//void ShowGradientEditor() {
//	ImGui::Text("Gradient Editor");
//
//	// 制御点の編集
//	for (size_t i = 0; i < gradientPoints.size(); ++i) {
//		ImGui::PushID(static_cast<int>(i));
//
//		ImGui::DragFloat("Position", &gradientPoints[i].position, 0.01f, 0.0f, 1.0f);
//		ImGui::ColorEdit4("Color", (float*)&gradientPoints[i].color);
//
//		// 削除ボタン
//		if (ImGui::Button("Remove")) {
//			ImGui::PopID(); // ★ PushID の対応を忘れずに
//			gradientPoints.erase(gradientPoints.begin() + i);
//			break;
//		}
//
//		ImGui::PopID(); // ★ 通常パスも忘れずに
//	}
//
//	// グラデーションのプレビュー
//	ImDrawList* drawList = ImGui::GetWindowDrawList();
//	ImVec2 canvasPos = ImGui::GetCursorScreenPos();
//	ImVec2 canvasSize = ImVec2(300, 50); // プレビューのサイズ
//	drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(50, 50, 50, 255));
//
//	for (size_t i = 0; i + 1 < gradientPoints.size(); ++i) {
//		ImVec2 start = ImVec2(canvasPos.x + gradientPoints[i].position * canvasSize.x, canvasPos.y);
//		ImVec2 end = ImVec2(canvasPos.x + gradientPoints[i + 1].position * canvasSize.x, canvasPos.y + canvasSize.y);
//		ImU32 colStart = ImGui::ColorConvertFloat4ToU32(gradientPoints[i].color);
//		ImU32 colEnd = ImGui::ColorConvertFloat4ToU32(gradientPoints[i + 1].color);
//		drawList->AddRectFilledMultiColor(start, end, colStart, colEnd, colEnd, colStart);
//	}
//
//	ImGui::Dummy(canvasSize); // スペースを確保
//
//	// コントロールポイント追加ボタン
//	if (ImGui::Button("Add Control Point")) {
//		GradientPoint newPoint;
//		newPoint.position = 0.5f;
//		newPoint.color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
//		gradientPoints.push_back(newPoint);
//		std::sort(gradientPoints.begin(), gradientPoints.end(), [](const GradientPoint& a, const GradientPoint& b) {
//			return a.position < b.position;
//			});
//	}
//
//	// コントロールポイントのドラッグ処理
//	const float knobRadius = 5.0f;
//	static int draggingIndex = -1;
//
//	for (size_t i = 0; i < gradientPoints.size(); ++i) {
//		ImGui::PushID(static_cast<int>(i));
//
//		float x = canvasPos.x + gradientPoints[i].position * canvasSize.x;
//		float y = canvasPos.y + canvasSize.y + 10.0f;
//		ImVec2 knobCenter = ImVec2(x, y);
//		drawList->AddCircleFilled(knobCenter, knobRadius, ImGui::ColorConvertFloat4ToU32(gradientPoints[i].color));
//
//		ImRect knobRect(ImVec2(x - knobRadius, y - knobRadius), ImVec2(x + knobRadius, y + knobRadius));
//		if (draggingIndex == -1 && ImGui::IsMouseHoveringRect(knobRect.Min, knobRect.Max) && ImGui::IsMouseClicked(0)) {
//			draggingIndex = static_cast<int>(i);
//		}
//
//		ImGui::PopID();
//	}
//
//	if (draggingIndex != -1 && ImGui::IsMouseDown(0)) {
//		float mouseX = ImGui::GetIO().MousePos.x;
//		float newPos = (mouseX - canvasPos.x) / canvasSize.x;
//		newPos = std::clamp(newPos, 0.0f, 1.0f);
//		gradientPoints[draggingIndex].position = newPos;
//
//		std::sort(gradientPoints.begin(), gradientPoints.end(), [](const GradientPoint& a, const GradientPoint& b) {
//			return a.position < b.position;
//			});
//	}
//	else if (draggingIndex != -1 && !ImGui::IsMouseDown(0)) {
//		draggingIndex = -1; // ドラッグ終了
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

	ParticleManager::GetInstance()->CreateParticleGroup("Particle_1", "Resources/circle.png",ParticleManager::ParticleType::Normal);

	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->Initialize("Particle_1");

	emitter_->SetPosition({ 0.0f,1.0f,0.0f });
	emitter_->SetParticleCount(1);
	emitter_->SetVelocityRange({ {-1.0f,1.0f},{1.0f,2.0f},{0.0f,0.0f} });
	emitter_->SetStartColorRange({ {1.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f},{1.0f,1.0f} });
	emitter_->SetFinishColorRange({ {1.0f,1.0f},{1.0f,1.0f},{1.0f,1.0f},{1.0f,1.0f} });
	emitter_->SetLifeTimeRange({ {1.5f,2.0f} });
	emitter_->SetFrequency(0.5f);

	emitter_->Emit();

	/*///========================================
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

	emitter_->Update();

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

	ImGui::Checkbox("useDebugCamera", &cameraManager_->useDebugCamera_);
	ImGui::Checkbox("sceneConticue", &isContinue);

	ImGui::DragFloat("colorSpace.hue", &colorSpace_->hue, 0.01f);
	ImGui::DragFloat("colorSpace.saturation", &colorSpace_->saturation, 0.01f);
	ImGui::DragFloat("colorSpace.value", &colorSpace_->value, 0.01f);

	//ImGui::DragFloat("dissolve.threshold", &dissolve_->threshold, 0.01f);
	//ImGui::DragFloat("dissolve.thresholdWidth", &dissolve_->thresholdWidth, 0.01f);

	ShowGradientEditor("Particle_1");

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