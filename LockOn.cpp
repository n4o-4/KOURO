#include "LockOn.h"
#include "Player.h"

void LockOn::Initialize() {

	//lockOn model
	lockOn_ = std::make_unique<Object3d>();
	lockOn_->Initialize(Object3dCommon::GetInstance());
	ModelManager::GetInstance()->LoadModel("lockOn/Lock_on.obj");	
	lockOn_->SetModel("lockOn/Lock_on.obj");

	lockOnWorldTransform_ = std::make_unique<WorldTransform>();
	lockOnWorldTransform_->Initialize();
	lockOnWorldTransform_->transform.translate = { 0.0f, 0.0f, 0.0f };
	
	// LockOnMarker 
	lockOnMarkers_.clear();

}

void LockOn::Update() {

#ifdef _DEBUG
	ImGui::Begin("LockOn Debug");
	ImGui::Text("Locked Enemies: %d", static_cast<int>(lockedEnemies_.size()));
	ImGui::End();

#endif

	lockOnWorldTransform_->UpdateMatrix();

	lockOn_->SetLocalMatrix(MakeIdentity4x4());
	lockOn_->Update();

	
	//検出された敵の数とLockOnMarkerの数を同期させる
	while (lockOnMarkers_.size() < lockedEnemies_.size()) {
		auto marker = std::make_unique<LockOnMarker>();
		marker->Initialize();
		lockOnMarkers_.push_back(std::move(marker));
	}
	while (lockOnMarkers_.size() > lockedEnemies_.size()) {
		lockOnMarkers_.pop_back();
	}
	//すべての検出された敵の前にLockOnMarkerを配置する
	for (size_t i = 0; i < lockedEnemies_.size(); ++i) {
		Vector3 enemyPos = lockedEnemies_[i]->GetPosition();
		Vector3 markerPos = { enemyPos.x, enemyPos.y + 1.0f, enemyPos.z - 1.0f }; 
		lockOnMarkers_[i]->SetTargetPosition(markerPos);
		lockOnMarkers_[i]->Show();
		lockOnMarkers_[i]->Update();
	}

}

void LockOn::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	lockOn_->Draw(*lockOnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	for (auto& marker : lockOnMarkers_) {
		marker->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}

void LockOn::DetectEnemies(const std::vector<std::unique_ptr<Enemy>>& enemies) {
	lockedEnemies_.clear();
	Vector3 lockOnPos = lockOnWorldTransform_->transform.translate;

	float detectionWidth = 1.0f;  
	float detectionHeight = 1.0f; 

	for (const auto& enemy : enemies) {
		Vector3 enemyPos = enemy->GetPosition();

		if (fabs(enemyPos.x - lockOnPos.x) < detectionWidth &&
			fabs(enemyPos.y - lockOnPos.y) < detectionHeight) {
			lockedEnemies_.push_back(enemy.get());
		}
	}

}



