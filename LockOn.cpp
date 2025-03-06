#include "LockOn.h"
#include "Player.h"
#include <iostream>

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

// LockOnのUpdateメソッド内のマーカー更新部分を修正
void LockOn::Update(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // 既存のデバッグ表示
#ifdef _DEBUG
    ImGui::Begin("LockOn Debug");
    ImGui::Text("Locked Enemies: %d", static_cast<int>(lockedEnemies_.size()));
    ImGui::End();
#endif

    DetectEnemies(enemies);
    
    lockOnWorldTransform_->UpdateMatrix();
    
    lockOn_->SetLocalMatrix(MakeIdentity4x4());
    lockOn_->Update();
    
    // マーカーの数をロックオン対象に合わせる
    while (lockOnMarkers_.size() < lockedEnemies_.size()) {
        auto marker = std::make_unique<LockOnMarker>();
        marker->Initialize();
        lockOnMarkers_.push_back(std::move(marker));
    }
    while (lockOnMarkers_.size() > lockedEnemies_.size()) {
        lockOnMarkers_.pop_back();
    }
    
    // プレイヤーの位置（ロックオンシステムの位置）を取得
    Vector3 playerPos = lockOnWorldTransform_->transform.translate;
    
    // 各マーカーを更新
    for (size_t i = 0; i < lockedEnemies_.size(); ++i) {
        Vector3 enemyPos = lockedEnemies_[i]->GetPosition();
        // マーカーを少し上に表示
        Vector3 markerPos = { enemyPos.x, enemyPos.y + 1.0f, enemyPos.z };
        
        // プレイヤー位置も渡して更新
        lockOnMarkers_[i]->Show();
        lockOnMarkers_[i]->Update(playerPos, markerPos);
    }
}

void LockOn::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	lockOn_->Draw(*lockOnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	for (auto& marker : lockOnMarkers_) {
		marker->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}

void LockOn::DetectEnemies(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // 既存のロックオンをクリア
    lockedEnemies_.clear();
    Vector3 lockOnPos = lockOnWorldTransform_->transform.translate;
    
    // カメラからの視点方向に基づくロックオン
    float detectionRange = 100.0f; // より広いロックオン距離
    float viewAngleThreshold = 0.93f; // より厳しい視点方向判定（約20度）
    
    // 視点方向が設定されていることを確認
    if (Length(viewDirection_) < 0.001f) {
        viewDirection_ = {0.0f, 0.0f, 1.0f}; // デフォルト前方向
    }

    // 視点優先度に基づく敵の並び替え用
    struct EnemyWithPriority {
        Enemy* enemy;
        float priority; // 視点との一致度
        float distance; // プレイヤーからの距離
    };
    std::vector<EnemyWithPriority> prioritizedEnemies;

    // 各敵について視点方向に基づく優先度を計算
    for (const auto& enemy : enemies) {
        Vector3 enemyPos = enemy->GetPosition();
        Vector3 toEnemy = enemyPos - lockOnPos;
        float distanceToEnemy = Length(toEnemy);
        
        // 距離が検出範囲内かチェック
        if (distanceToEnemy > 0.001f && distanceToEnemy < detectionRange) {
            // 方向ベクトルを正規化
            Vector3 normalizedToEnemy = {
                toEnemy.x / distanceToEnemy,
                toEnemy.y / distanceToEnemy,
                toEnemy.z / distanceToEnemy
            };
            
            // 視点方向との一致度を計算
            float dotProduct = Dot(viewDirection_, normalizedToEnemy);
            
            // 視点方向との一致度が閾値を超える場合、リストに追加
            if (dotProduct > viewAngleThreshold) {
                prioritizedEnemies.push_back({
                    enemy.get(), 
                    dotProduct,    // 視点との一致度
                    distanceToEnemy // 距離
                });
            }
        }
    }

    // 視点との一致度でソート（高い順）
    std::sort(prioritizedEnemies.begin(), prioritizedEnemies.end(), 
        [](const EnemyWithPriority& a, const EnemyWithPriority& b) {
            // 視点との一致度が同じなら距離で比較
            if (std::abs(a.priority - b.priority) < 0.01f) {
                return a.distance < b.distance;
            }
            return a.priority > b.priority;
        });

    // 最大3体までロックオン対象に追加
    const size_t maxLockCount = 3;
    for (size_t i = 0; i < std::min(prioritizedEnemies.size(), maxLockCount); ++i) {
        lockedEnemies_.push_back(prioritizedEnemies[i].enemy);
    }
}

void LockOn::RemoveLockedEnemy(Enemy* enemy)
{
	auto it = std::remove(lockedEnemies_.begin(), lockedEnemies_.end(), enemy);
	lockedEnemies_.erase(it, lockedEnemies_.end());

	// マーカーの数を同期
	while (lockOnMarkers_.size() > lockedEnemies_.size()) {
		lockOnMarkers_.pop_back();
	}
}



