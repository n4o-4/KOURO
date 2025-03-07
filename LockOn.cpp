#include "LockOn.h"
#include "Player.h"
#include <iostream>
#include <cmath> // 数学関数用

///=============================================================================
///                        初期化
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

///=============================================================================
///                        更新処理
void LockOn::Update(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // 既存のデバッグ表示
#ifdef _DEBUG
    ImGui::Begin("LockOn Debug");
    ImGui::Text("Locked Enemies: %d", static_cast<int>(lockedEnemies_.size()));
    ImGui::End();
#endif

    DetectEnemies(enemies);
    
    // プレイヤーの位置（ロックオンシステムの位置）を取得
    Vector3 playerPos = lockOnWorldTransform_->transform.translate;
    
    // カメラの視点方向に基づいてメインロックオンを配置
    // カメラ視点方向の少し前にロックオンマークを表示
    float distanceInFront = 20.0f; // マークを表示する距離
    
    // 視点方向が有効か確認
    if (Length(viewDirection_) > 0.001f) {
        // 正規化された視点方向
        Vector3 normalizedDirection = Normalize(viewDirection_);
        
        // プレイヤーの前方に配置
        Vector3 lockOnPosition = {
            playerPos.x + normalizedDirection.x * distanceInFront,
            playerPos.y + normalizedDirection.y * distanceInFront,
            playerPos.z + normalizedDirection.z * distanceInFront
        };
        
        // ロックオンの位置を設定
        lockOnWorldTransform_->transform.translate = lockOnPosition;
        
        // 視点方向の逆向きを計算（ロックオンがカメラを向くように）
        Vector3 toCamera = {
            -normalizedDirection.x,
            -normalizedDirection.y,
            -normalizedDirection.z
        };
        
        // 回転角を計算
        float yaw = std::atan2(toCamera.x, toCamera.z);
        float pitch = std::atan2(-toCamera.y, std::sqrt(toCamera.x * toCamera.x + toCamera.z * toCamera.z));
        
        // ロックオンの回転を設定
        lockOnWorldTransform_->transform.rotate = { pitch, yaw, 0.0f };
    }
    
    // 行列を更新
    lockOnWorldTransform_->UpdateMatrix();
    lockOn_->SetLocalMatrix(MakeIdentity4x4());
    lockOn_->Update();
    
    // マーカーの処理（既存コード）
    // マーカーの数をロックオン対象に合わせる
    while (lockOnMarkers_.size() < lockedEnemies_.size()) {
        auto marker = std::make_unique<LockOnMarker>();
        marker->Initialize();
        marker->Show(); // 新しく作成したマーカーのみShow()を呼び出す
        lockOnMarkers_.push_back(std::move(marker));
    }
    while (lockOnMarkers_.size() > lockedEnemies_.size()) {
        lockOnMarkers_.pop_back();
    }
    
    // 各マーカーを更新
    for (size_t i = 0; i < lockedEnemies_.size(); ++i) {
        Vector3 enemyPos = lockedEnemies_[i]->GetPosition();
        // マーカーを少し上に表示
        Vector3 markerPos = { enemyPos.x, enemyPos.y + 1.0f, enemyPos.z };
        
        // 視点方向を渡して更新
        lockOnMarkers_[i]->Update(playerPos, markerPos);
    }
    
}
///=============================================================================
///                        描画処理
void LockOn::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	lockOn_->Draw(*lockOnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	for (auto& marker : lockOnMarkers_) {
		marker->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}
///=============================================================================
///                        ロックオン範囲内の敵を検知する関数
void LockOn::DetectEnemies(const std::vector<std::unique_ptr<Enemy>>& enemies) {
	//========================================
    // 既存のロックオンをクリア
    lockedEnemies_.clear();
    Vector3 lockOnPos = lockOnWorldTransform_->transform.translate;
    //========================================
    // 視点方向が設定されていることを確認
    if (Length(viewDirection_) < 0.001f) {
        viewDirection_ = {0.0f, 0.0f, 1.0f}; // デフォルト前方向
    }
	//========================================
    // 視点優先度に基づく敵の並び替え用
    struct EnemyWithPriority {
        Enemy* enemy;
        float priority; // 視点との一致度
        float distance; // プレイヤーからの距離
    };
    std::vector<EnemyWithPriority> prioritizedEnemies;
	//========================================
    // 各敵について視点方向に基づく優先度を計算
    for (const auto& enemy : enemies) {
        Vector3 enemyPos = enemy->GetPosition();
        Vector3 toEnemy = enemyPos - lockOnPos;
        float distanceToEnemy = Length(toEnemy);
        
        // 距離が検出範囲内かチェック
        if (distanceToEnemy > 0.001f && distanceToEnemy < detectionRange_) {
            // 方向ベクトルを正規化
            Vector3 normalizedToEnemy = {
                toEnemy.x / distanceToEnemy,
                toEnemy.y / distanceToEnemy,
                toEnemy.z / distanceToEnemy
            };
            
            // 視点方向との一致度を計算
            float dotProduct = Dot(viewDirection_, normalizedToEnemy);
            
            // 視点方向との一致度が閾値を超える場合、リストに追加
            if (dotProduct > viewAngleThreshold_) {
                prioritizedEnemies.push_back({
                    enemy.get(), 
                    dotProduct,    // 視点との一致度
                    distanceToEnemy // 距離
                });
            }
        }
    }
	//========================================
    // 視点との一致度でソート（高い順）
    std::sort(prioritizedEnemies.begin(), prioritizedEnemies.end(), 
        [](const EnemyWithPriority& a, const EnemyWithPriority& b) {
            // 視点との一致度が同じなら距離で比較
            if (std::abs(a.priority - b.priority) < 0.01f) {
                return a.distance < b.distance;
            }
            return a.priority > b.priority;
        });
    lockedEnemies_.clear(); // 既存のリストをクリア
	//========================================
	// ロックオン対象の数を制限
    for (size_t i = 0; i < std::min(prioritizedEnemies.size(), maxLockCount_); ++i) {
        lockedEnemies_.push_back(prioritizedEnemies[i].enemy);
    }
}
///=============================================================================
///                        ロックオンされている敵のリストを取得
void LockOn::RemoveLockedEnemy(Enemy* enemy)
{
	auto it = std::remove(lockedEnemies_.begin(), lockedEnemies_.end(), enemy);
	lockedEnemies_.erase(it, lockedEnemies_.end());

	// マーカーの数を同期
	while (lockOnMarkers_.size() > lockedEnemies_.size()) {
		lockOnMarkers_.pop_back();
	}
}



