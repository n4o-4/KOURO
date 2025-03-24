#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include <iostream>
#include <list>
#include <memory>
#include <random>

#include "EnemyBullet.h"

class BaseEnemy : public BaseObject {
    ///--------------------------------------------------------------
    ///						 メンバ関数 
public:

    virtual ~BaseEnemy() = default;

    // @brief 初期化
    virtual void Initialize(Model* model);
    // @brief 更新
    virtual void Update();
    /**----------------------------------------------------------------------------
     * \brief  Draw 描画
     * \param  viewProjection ビュー射影行列
     * \param  directionalLight ディレクショナルライト
     * \param  pointLight ポイントライト
     * \param  spotLight スポットライト
     */
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);


    const Vector3& GetPosition() const {
        return worldTransform_->transform.translate;
    }

    std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

    virtual WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

    /**----------------------------------------------------------------------------
   * \brief  GetHp HPを取得
   * \return HP
   */
    const int GetHp() const { return hp_; }
    ///--------------------------------------------------------------
    ///						 当たり判定
private:
    /// \brief 衝突開始時の処理
    void OnCollisionEnter(BaseObject* other) override;

    /// \brief 衝突継続時の処理
    void OnCollisionStay(BaseObject* other) override;

    /// \brief 衝突終了時の処理
    void OnCollisionExit(BaseObject* other) override;

    ///--------------------------------------------------------------
    ///						 入出力関数
protected:
  
    // 方向を選択するメソッド
    Vector3 SelectDirection();

    void BulletUpdate();
    void BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);
    void Fire();

    void MoveToTarget();

    void RandomMove();

    ////
    void SetModelColor(const Vector4& color) { if (model_) model_->SetColor(color); }
    
    ///--------------------------------------------------------------
    /// メンバ変数
    protected:
    //========================================
    // モデル
    std::unique_ptr<Object3d> model_ = nullptr;
    
    //ワールド変換
    std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

    //HP
    int hp_ = 1;

    // 移動関連
    float speed_ = 0.15f;   // 移動速度
    float minX_ = -100.0f;  // 左の限界
    float maxX_ = 100.0f;   // 右の限界
    int direction_ = 1;     // 移動方向 (1:右, -1:左)
    Vector3 velocity_ = { 0.0f, 0.0f, 0.0f }; // 現在の速度ベクトル
    Vector3 spawnPosition_ = { 0.0f, 0.0f, 0.0f }; // スポーン位置

    // 行動状態
    float stateTimer_ = 0.0f;
    float directionChangeTimer_ = 0.0f;

    // 行動パラメータ
    float chaseDistance_ = 64.0f;    // この距離内ならプレイヤーを追いかける
    float combatDistance_ = 32.0f;   // 戦闘を行う距離
    float safeDistance_ = 10.0f;     // 安全距離（この距離を保つ）
    float wanderRadius_ = 32.0f;     // 徘徊半径
    float directionChangeInterval_ = 3.0f; // 方向変更の間隔

    // 乱数生成器
    std::mt19937 rng_;
    std::uniform_real_distribution<float> angleDist_{ 0.0f, 2.0f * 3.14159f };
    std::uniform_real_distribution<float> strengthDist_{ 0.5f, 1.0f };

    WorldTransform* target_;
    std::list<std::unique_ptr<EnemyBullet>> bullets_;
    const float kIntervalTiem = 5.0f;
    float intervalCounter_ = 0.0f;

    //Spawn model
    std::unique_ptr<Object3d> spawnModel_ = nullptr;
    //Spawn ワールド変換
    std::unique_ptr<WorldTransform> spawnWorldTransform_ = nullptr;

};