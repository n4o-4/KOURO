#include "PakuScene.h"



void PakuScene::Initialize()
{
	BaseScene::Initialize();


	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();
	
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize();

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	LoadEnemyPopData();
}

void PakuScene::Finalize()
{

	skyDome_.reset();

	ground_.reset();

	enemy_.reset();
}

void PakuScene::Update()
{

#ifdef _DEBUG
	if (ImGui::TreeNode("Camera")) {
		//ImGui::DragFloat3("camera", &, 0.01f);

		ImGui::TreePop(); // TreeNodeを閉じる
	}

#endif

	BaseScene::Update();
	skyDome_->Update();
	ground_->Update();
	enemy_->Update();
	//
	UpdateEnemyPopCommands();
	for (const auto& enemy : enemies_) {
		enemy->Update();
	}
}

void PakuScene::Draw()
{
	DrawBackgroundSprite();
	/// 背景スプライト描画


	DrawObject();
	/// オブジェクト描画	

	skyDome_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

	ground_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

	//enemy_->Draw(Camera::GetInstance()->GetViewProjection(),
	//	*directionalLight.get(),
	//	*pointLight.get(),
	//	*spotLight.get());

	for (const auto& enemy : enemies_) {
		enemy->Draw(Camera::GetInstance()->GetViewProjection(),
			*directionalLight.get(),
			*pointLight.get(),
			*spotLight.get());
	}

	DrawForegroundSprite();
	/// 前景スプライト描画	

	

}

void PakuScene::LoadEnemyPopData() {
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void PakuScene::UpdateEnemyPopCommands() {
	if (isWaiting_) {
		--waitTimer_;
		if (--waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		return;
	}
	std::string line;

	while (getline(enemyPopCommands, line)) {

		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			SpawnEnemy(Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaiting_ = true;
			waitTimer_ = waitTime;

			break;

		}


	}
}

void PakuScene::SpawnEnemy(const Vector3& position) {

	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize();
	newEnemy->SetPosition(position);
	//newEnemy->SetPlayer(player_);
	enemies_.push_back(std::move(newEnemy));

}
