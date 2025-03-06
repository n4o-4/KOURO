#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include "Enemy.h"
#include "LockOnMarker.h"

class LockOn {

public:

	void Initialize(); 
	void Update(const std::vector<std::unique_ptr<Enemy>>& enemies);
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetPosition(const Vector3& position) { lockOnWorldTransform_->transform.translate = position; }
	// LockOn 範囲内の敵を検知する関数
	void DetectEnemies(const std::vector<std::unique_ptr<Enemy>>& enemies);
	//検出された敵のリストを取得する関数
	size_t GetLockedEnemyCount() const { return lockedEnemies_.size(); }

	// 視点方向を設定
	void SetViewDirection(const Vector3& viewDir) { viewDirection_ = viewDir; }

	/**----------------------------------------------------------------------------
     * \brief  GetLockedEnemies ロックオンされている敵のリストを取得
     * \return ロックオンされている敵のリスト
     */
	const std::vector<Enemy*>& GetLockedEnemies() const { return lockedEnemies_; }

	void RemoveLockedEnemy(Enemy* enemy);


private:
	//========================================
	//lockOn model
	std::unique_ptr<Object3d> lockOn_ = nullptr;
	//ワールド変換
	std::unique_ptr<WorldTransform> lockOnWorldTransform_ = nullptr;
	//========================================
	// 検出された敵のリスト
	std::vector<Enemy*> lockedEnemies_;
	//========================================
	//検出されたEnemyの前に表示されるオブジェクト
	std::vector<std::unique_ptr<LockOnMarker>> lockOnMarkers_;
	//========================================
	//敵
	Enemy* enemy_ = nullptr;
	//========================================
	// 視点方向ベクトル
	Vector3 viewDirection_ = { 0.0f, 0.0f, 1.0f }; // デフォルトは前方向
	//========================================
	// ロックオン範囲
	float detectionRange_ = 100.0f;   // より広いロックオン距離
    float viewAngleThreshold_ = 0.99f; // より厳しい視点方向判定（約20度）
	const size_t maxLockCount_ = 3;	  // 最大3体までロックオン対象に追加
};

