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

	/**----------------------------------------------------------------------------
     * \brief  GetLockedEnemies ロックオンされている敵のリストを取得
     * \return ロックオンされている敵のリスト
     */
	const std::vector<Enemy*>& GetLockedEnemies() const { return lockedEnemies_; }


private:

	//lockOn model
	std::unique_ptr<Object3d> lockOn_ = nullptr;
	//ワールド変換
	std::unique_ptr<WorldTransform> lockOnWorldTransform_ = nullptr;

	// 検出された敵のリスト
	std::vector<Enemy*> lockedEnemies_;

	//検出されたEnemyの前に表示されるオブジェクト
	std::vector<std::unique_ptr<LockOnMarker>> lockOnMarkers_;

};

