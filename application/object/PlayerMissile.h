#pragma once
#include "BaseObject.h"
#include "Enemy.h"

/// @brief プレイヤーのミサイルを制御するクラス
class PlayerMissile : public BaseObject {
public:
    /// @brief コンストラクタ
    /// @param position 初期位置
    /// @param initialVelocity 初期速度
    /// @param scale モデルのスケール
    /// @param rotate モデルの回転
    PlayerMissile(const Vector3 &position, const Vector3 &initialVelocity,
        const Vector3 &scale, const Vector3 &rotate);

    /// @brief 毎フレームの更新処理
    void Update();

    /// @brief 描画処理
    /// @param viewProjection ビュープロジェクション
    /// @param directionalLight 平行光源
    /// @param pointLight 点光源
    /// @param spotLight スポットライト
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
        PointLight pointLight, SpotLight spotLight);

    /// @brief ターゲットの設定
    /// @param target 追尾対象の敵
    void SetTarget(Enemy *target) { target_ = target; }

    /// @brief アクティブ状態の取得
    /// @return ミサイルが活動中かどうか
    bool IsActive() const { return isActive_; }

private:
    //===================================================
    // 当たり判定関連メソッド
    /// @brief 衝突開始時の処理
    /// @param other 衝突相手のオブジェクト
    void OnCollisionEnter(BaseObject *other) override;

    /// @brief 衝突継続時の処理
    /// @param other 衝突相手のオブジェクト
    void OnCollisionStay(BaseObject *other) override;

    /// @brief 衝突終了時の処理
    /// @param other 衝突相手のオブジェクト
    void OnCollisionExit(BaseObject *other) override;

private:
    //===================================================
    // 列挙型の定義
    /// @brief ミサイルの飛行状態
    enum class BulletState {
        kLaunch,    // 発射初期段階：安定飛行と目標への準備
        kTracking,  // 追尾段階：目標をロックオン・追尾
        kFinal      // 最終接近段階：軌道修正と目標への突入
    };

    //===================================================
    // 定数定義
    // 全般的なパラメータ
    /// @brief 弾の最大寿命（フレーム数）
    static const int kLifeTimeLimit = 300;

    // 発射初期段階のパラメータ
    /// @brief 発射初期段階の持続時間
    static const int kLaunchStateTransitionTime = 60;
    /// @brief 初期速度の増加率の調整用
    static constexpr float kLaunchSpeedDivider = 40.0f;
    /// @brief 発射直後の初期速度
    static constexpr float kInitialSpeed = 0.3f;
    /// @brief 初期状態での速度増加量
    static constexpr float kSpeedIncreaseFactor = 0.15f;
    /// @brief 慣性飛行の持続時間（フレーム数）
    static const int kInertialFlightTime = 20;
    /// @brief 初期段階でのランダム動作の大きさを調整
    static constexpr float kRandomMoveFactor = 1500.0f;
    /// @brief ランダム動作の頻度を調整（フレーム）
    static const int kRandomMoveInterval = 8;

    // 追尾段階のパラメータ
    /// @brief 弧を描く動きの速さ
    static constexpr float kArcOscillationSpeed = 0.02f;
    /// @brief 弧の大きさ
    static constexpr float kArcStrength = 0.01f;
    /// @brief 追尾時の基本速度
    static constexpr float kBaseTrackingSpeed = 1.8f;
    /// @brief 時間経過による最大速度の増加量
    static constexpr float kMaxSpeedIncrease = 0.5f;
    /// @brief 時間経過による速度増加の調整用
    static constexpr float kSpeedIncreaseDivider = 30.0f;
    /// @brief 比例航法（PN）のナビゲーションゲイン
    static constexpr float kNavigationGain = 3.0f;
    /// @brief ターゲットに接近したと判断する距離
    static constexpr float kFinalStateTransitionDistance = 15.0f;
    /// @brief 追尾状態の最大時間（フレーム数）
    static const int kAutoFinalStateTransitionTime = 60;
    /// @brief 追尾遅延のシミュレーション係数（小さいほど反応が遅い）
    static constexpr float kTrackingDelayFactor = 0.85f;
    /// @brief 滑らかさ調整係数（小さいほど滑らかに変化）
    static constexpr float kSmoothingFactor = 0.15f;
    /// @brief 旋回制限係数（小さいほど緩やかに旋回）
    static constexpr float kTurnLimitFactor = 0.7f;

    // 最終接近段階のパラメータ
    /// @brief 最終接近時の基本速度
    static constexpr float kBaseFinalSpeed = 1.8f;
    /// @brief 時間経過による最終速度の増加量
    static constexpr float kMaxFinalSpeedIncrease = 0.7f;
    /// @brief 時間経過による最終速度増加の調整用
    static constexpr float kFinalSpeedIncreaseDivider = 8.0f;
    /// @brief 最終段階での旋回率増加係数
    static constexpr float kFinalTurnRateFactor = 1.5f;
    /// @brief ランダム挙動開始フレーム
    static const int kFinalRandomBehaviorStartFrame = 10;
    /// @brief ランダム偏差係数
    static constexpr float kRandomDeviationFactor = 2000.0f;

    // 比例航法と誘導関連のパラメータ
    /// @brief 曲線運動の振動速度
    static constexpr float kCurveOscillationSpeed = 0.025f;
    /// @brief 曲線運動の強さ
    static constexpr float kCurveStrength = 0.03f;
    /// @brief 垂直方向成分の影響度
    static constexpr float kVelocityPerpWeight = 0.01f;
    /// @brief 比例航法のゲイン減少係数
    static constexpr float kNavGainReductionFactor = 0.6f;
    /// @brief 操舵力のゲイン減少係数
    static constexpr float kSteeringGainReductionFactor = 0.75f;

    // 近接信管関連
    /// @brief 近接信管の発動距離
    static constexpr float kProximityFuseDistance = 3.0f;

    //===================================================
    // メンバ変数
    // 状態管理
    /// @brief 現在のミサイル状態
    BulletState state_ = BulletState::kLaunch;
    /// @brief 各状態の経過時間
    int stateTimer_ = 0;
    /// @brief アクティブ状態
    bool isActive_ = true;
    /// @brief 経過時間（総フレーム数）
    int lifeTime_ = 0;
    
    // 物理量
    /// @brief 速度ベクトル
    Vector3 velocity_;
    /// @brief 最大速度
    float maxSpeed_ = 4.5f;
    /// @brief 加速度
    float acceleration_ = 2.01f;
    /// @brief 回転角度
    float rotationAngle_ = 0.0f;
    /// @brief 角速度
    float angularVelocity_ = 0.0f;
    
    // 誘導システム
    /// @brief 比例航法のゲイン
    float navigationGain_ = 0.001f;
    /// @brief 前回の視線方向（視線角速度計算用）
    Vector3 prevLineOfSight_;
    /// @brief 旋回性能の限界
    float maxTurnRate_ = 0.05f;
    
    // 近接信管
    /// @brief 近接信管の発動フラグ
    bool proximityFused_ = false;
    
    // 性能要素
    /// @brief ミサイル性能のランダムばらつき（0.64〜1.28の範囲）
    float performanceVariation_;
    
    // グラフィック
    /// @brief ミサイルの3Dモデル
    std::unique_ptr<Object3d> model_;
    /// @brief ワールド変換情報
    std::unique_ptr<WorldTransform> worldTransform_;
    
    // ターゲット
    /// @brief 追尾対象
    Enemy *target_ = nullptr;

    //===================================================
    // 内部メソッド
    /// @brief 発射初期段階の更新処理
    void UpdateLaunchState();
    
    /// @brief 追尾段階の更新処理
    void UpdateTrackingState();
    
    /// @brief 最終接近段階の更新処理
    void UpdateFinalState();
    
    /// @brief 比例航法ベクトルの計算
    /// @return 誘導ベクトル
    Vector3 CalculateNavigationVector();
    
    /// @brief 視線ベクトル（LOS: Line of Sight）の計算
    /// @return ターゲットへの視線ベクトル
    Vector3 CalculateLOS();
    
    /// @brief 視線角速度（LOS Rate）の計算
    /// @return 視線角速度ベクトル
    Vector3 CalculateLOSRate();
    
    /// @brief 近接信管の処理
    void ApplyProximityFuse();
};