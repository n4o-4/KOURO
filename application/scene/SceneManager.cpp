#include "SceneManager.h"
#include <assert.h>

std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance =  std::make_unique<SceneManager>();
	}

	return instance.get();
}

void SceneManager::Initialize(SrvManager* srvManager, Camera* camera)
{
	srvManager_ = srvManager;

	camera_ = camera;
}

void SceneManager::Finalize()
{
	instance.reset();
}

void SceneManager::Update()
{
	// シーンを追加した場合はここに追加
	const std::vector<std::string> sceneNames = { "TITLE", "GAME", "OKA","PAKU","MARU","MIYA","CLEAR","OVER"};

	static int currentSceneIndex = 0;

	ImGui::Begin("Scene");

	if (ImGui::BeginCombo("Scene", sceneNames[currentSceneIndex].c_str()))
	{
		for (int i = 0; i < sceneNames.size(); i++)
		{
			bool isSelected = (currentSceneIndex == i);
			if (ImGui::Selectable(sceneNames[i].c_str(), isSelected))
			{
				currentSceneIndex = i;

				ChangeScene(sceneNames[i]);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();


	if (nextScene_)
	{
		// 
		if (scene_)
		{
			scene_.reset();
			scene_ = nullptr;
		}

		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;
		
		scene_->Initialize();

		scene_->SetSceneManager(this);
		scene_->SetSrvManager(srvManager_);
		scene_->SetCamera(camera_);
	}

	// 実行中のシーンの更新
	scene_->Update();
}

void SceneManager::Draw()
{

	// 実行中のシーンの描画
	scene_->Draw();

}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次のシーン生成
	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));
}
