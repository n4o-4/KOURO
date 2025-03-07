// 既存のヘッダファイルに追加
#pragma once
#include "BaseObject.h"
#include "Enemy.h"

class PlayerBullet : public BaseObject {
public:
    // コンストラクタ
    PlayerBullet(const Vector3& position, const Vector3& initialVelocity, 
                 const Vector3& scale, const Vector3& rotate);
    
    void Update();
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, 
              PointLight pointLight, SpotLight spotLight);
    
    // ターゲットの設定
    void SetTarget(Enemy* target) { target_ = target; }
    
    // アクティブ状態の取得
    bool IsActive() const { return isActive_; }

    ///--------------------------------------------------------------
    ///						 当たり判定
private:
    /// \brief 衝突開始時の処理
    void OnCollisionEnter(BaseObject *other) override;

    /// \brief 衝突継続時の処理
    void OnCollisionStay(BaseObject *other) override;

    /// \brief 衝突終了時の処理
    void OnCollisionExit(BaseObject *other) override;

private:
    // 弾の状態
    enum class BulletState {
        kLaunch,    // 発射初期段階
        kTracking,  // 追尾段階
        kFinal      // 最終接近段階
    };
    
    // 現在の弾の状態
    BulletState state_ = BulletState::kLaunch;
    
    // 各状態の経過時間
    int stateTimer_ = 0;
    
    // 速度ベクトル
    Vector3 velocity_;
    
    // 最大速度と加速度
    float maxSpeed_ = 4.5f;
    float acceleration_ = 2.01f;
    
    // 回転角度と角速度
    float rotationAngle_ = 0.0f;
    float angularVelocity_ = 0.0f;
    
    // 追尾パラメータ
    float navigationGain_ = 0.9f;  // 比例航法のゲイン
    
    // モデルとトランスフォーム
    std::unique_ptr<Object3d> model_;
    std::unique_ptr<WorldTransform> worldTransform_;
    
    // ターゲット
    Enemy* target_ = nullptr;
    
    // アクティブ状態
    bool isActive_ = true;
    
    // 経過時間
    int lifeTime_ = 0;
    
    // メソッド
    void UpdateLaunchState();
    void UpdateTrackingState();
    void UpdateFinalState();
    Vector3 CalculateNavigationVector();
};