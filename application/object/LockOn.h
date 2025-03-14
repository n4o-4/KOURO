#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include "Enemy.h"
#include "LockOnMarker.h"

class LockOn {
public:
    // ロックオンレベルを表す列挙型
    enum class LockLevel {
        None,           // ロックオンなし
        SimpleLock,     // 簡易ロックオン
        PreciseLock     // 精密ロックオン
    };
    
    // 敵ごとのロックオン情報を管理する構造体
	struct LockedEnemyInfo {
    	Enemy* enemy;                 // ロックオン対象の敵
    	float lockTimer;              // ロックオン継続時間
	    LockLevel lockLevel;          // 現在のロックオンレベル
	    float viewAlignmentFactor;    // 視点との整列度（0.0～1.0）
	    Vector3 initialPosition;      // 初回ロックオン時の敵の位置（簡易ロックオン用）
	};

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
    
    // ロックオン対象の敵を削除
    void RemoveLockedEnemy(Enemy* enemy);
    
    // 特定の敵のロックオンレベルを取得
    LockLevel GetLockLevel(Enemy* enemy) const;
    
    // 特定の敵のロックオン情報を取得
    LockedEnemyInfo* GetLockedEnemyInfo(Enemy* enemy);
    
    // ロックオン対象リストを取得
    const std::vector<Enemy*>& GetLockedEnemies() const { return lockedEnemies_; }
    
    // 精密ロックオン移行時間の取得
    float GetTimeToPreciseLock() const { return timeToPreciseLock_; }

	// 特定の敵の追尾位置を取得（簡易ロックオンでは初期位置を返す）
Vector3 GetLockedEnemyPosition(Enemy* enemy) const;

private:
    //lockOn model
    std::unique_ptr<Object3d> lockOn_ = nullptr;
    //ワールド変換
    std::unique_ptr<WorldTransform> lockOnWorldTransform_ = nullptr;
    
    // 検出された敵のリスト
    std::vector<Enemy*> lockedEnemies_;
    
    // 敵ごとのロックオン情報リスト
    std::vector<LockedEnemyInfo> lockedEnemiesInfo_;
    
    //検出されたEnemyの前に表示されるオブジェクト
    std::vector<std::unique_ptr<LockOnMarker>> lockOnMarkers_;
    
    //敵
    Enemy* enemy_ = nullptr;
    
    // 視点方向ベクトル
    Vector3 viewDirection_ = { 0.0f, 0.0f, 1.0f }; // デフォルトは前方向
    
    // ロックオン範囲
    float detectionRange_ = 400.0f;      // ロックオン検出距離
    float viewAngleThreshold_ = 0.99f;   // 視点方向判定（約8度）
    const size_t maxLockCount_ = 8;    // 最大ロックオン数
    
    // ロックオンレベルの遷移タイミング
    float timeToSimpleLock_ = 0.0f;      // 簡易ロックオンまでの時間（即時）
    float timeToPreciseLock_ = 2.0f;     // 精密ロックオンまでの時間（秒）
    
    // 視点中心との整列度の計算
    float CalculateViewAlignmentFactor(float dotProduct);
};