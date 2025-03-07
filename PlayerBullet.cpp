#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include "PlayerBullet.h"

// コンストラクタ実装
PlayerBullet::PlayerBullet(const Vector3& position, const Vector3& initialVelocity, 
                          const Vector3& scale, const Vector3& rotate) {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());
    //
	ModelManager::GetInstance()->LoadModel("player.obj");
	model_->SetModel("player.obj");

	isActive_ = true;  // 明示的にアクティブに設定
    
    // トランスフォームの初期化
    worldTransform_ = std::make_unique<WorldTransform>();
    worldTransform_->Initialize();
    worldTransform_->transform.translate = position;
    worldTransform_->transform.scale = scale;
    worldTransform_->transform.rotate = rotate;

	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);
}

void PlayerBullet::Update() {
    // ライフタイムを更新
    lifeTime_++;
    
    // 状態に応じた更新処理
    switch (state_) {
    case BulletState::kLaunch:
        UpdateLaunchState();
        break;
    case BulletState::kTracking:
        UpdateTrackingState();
        break;
    case BulletState::kFinal:
        UpdateFinalState();
        break;
    }
    
    // 位置の更新
    worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
    model_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
    worldTransform_->UpdateMatrix();
    
    // 当たり判定の更新
    BaseObject::Update(worldTransform_->transform.translate);
    
    // ライフタイム経過によるチェック（寿命）
    if (lifeTime_ > 600) { // 10秒間
        isActive_ = false;
    }
    
    // ターゲットが無効になった場合のチェック - 削除または修正
    // 以下の条件文を削除して、代わりにターゲットがいない場合の処理をする
    if (target_ && target_->GetHp() <= 0) {
        // ターゲットが無効になった場合、直進するために状態を維持
        // ターゲットポインタをnullptrにすることで、UpdateTrackingState()などの処理で直進するようになる
        target_ = nullptr;
        
        // 状態が最終状態でない場合は直進状態に切り替える
        if (state_ == BulletState::kTracking) {
            // 現在速度のまま直進する
            velocity_ = Normalize(velocity_) * 0.3f;
        }
        
        // 弾を非アクティブにするコードを削除
        // isActive_ = false; 
    }
}

// 発射初期状態の更新
void PlayerBullet::UpdateLaunchState() {
    // タイマー更新
    stateTimer_++;
    
    // 初期状態では、ほぼ直線的に進む
    // 速度を徐々に増加させる
    float speedFactor = std::min(1.0f, stateTimer_ / 30.0f);
    float currentSpeed = 0.2f + speedFactor * 0.1f;
    velocity_ = Normalize(velocity_) * currentSpeed;
    
    // わずかにランダムな動きを加える（揺らぎ）
    if (stateTimer_ % 5 == 0) {
        float randomX = (rand() % 100 - 50) / 1000.0f;
        float randomY = (rand() % 100 - 50) / 1000.0f;
        velocity_ = velocity_ + Vector3{randomX, randomY, 0.0f};
        velocity_ = Normalize(velocity_) * currentSpeed;
    }
    
    // 一定時間経過後、トラッキング状態に移行
    if (stateTimer_ >= 45) {
        state_ = BulletState::kTracking;
        stateTimer_ = 0;
    }
}

// 追尾状態の更新
void PlayerBullet::UpdateTrackingState() {
    stateTimer_++;
    
    if (target_ && target_->GetHp() > 0) {
        // ターゲットの位置を取得
        Vector3 targetPos = target_->GetPosition();
        Vector3 direction = targetPos - worldTransform_->transform.translate;
        float distance = Length(direction);
        
        // 比例航法による誘導
        Vector3 navVector = CalculateNavigationVector();
        
        // 弧を描くような動きを追加
        float arcFactor = sinf(stateTimer_ * 0.05f) * 0.03f;
        Vector3 perpVector = Cross(Normalize(velocity_), Vector3{0, 1, 0});
        perpVector = perpVector * arcFactor;
        
        // 速度ベクトルを更新
        velocity_ = velocity_ + navVector + perpVector;
        
        // 速度の大きさを調整
        float currentMaxSpeed = 0.3f + std::min(0.2f, stateTimer_ / 120.0f);
        velocity_ = Normalize(velocity_) * currentMaxSpeed;
        
        // 近づいたら最終段階へ
        if (distance < 10.0f) {
            state_ = BulletState::kFinal;
            stateTimer_ = 0;
        }
    }
    else {
        // ターゲットがいない場合は直進
        velocity_ = Normalize(velocity_) * 0.3f;
    }
    
    // 長時間追尾してもターゲットに到達できない場合
    if (stateTimer_ > 300) {
        state_ = BulletState::kFinal;
        stateTimer_ = 0;
    }
}

// 最終接近状態の更新
void PlayerBullet::UpdateFinalState() {
    stateTimer_++;
    
    if (target_ && target_->GetHp() > 0) {
        // ターゲットへの方向
        Vector3 targetPos = target_->GetPosition();
        Vector3 direction = targetPos - worldTransform_->transform.translate;
        
        // 急加速して直進
        float finalSpeed = 0.5f + std::min(0.5f, stateTimer_ / 10.0f);
        velocity_ = Normalize(direction) * finalSpeed;
    }
}

// 比例航法ベクトルの計算
Vector3 PlayerBullet::CalculateNavigationVector() {
    if (!target_ || target_->GetHp() <= 0) {
        return Vector3{0, 0, 0};
    }
    
    // ターゲットの位置
    Vector3 targetPos = target_->GetPosition();
    
    // 自分の位置とターゲットへの方向
    Vector3 myPos = worldTransform_->transform.translate;
    Vector3 lineOfSight = targetPos - myPos;
    
    // 視線の変化率（比例航法の基本）
    Vector3 relativeVelocity = velocity_; // ターゲットの速度は考慮しない簡易版
    
    // 視線方向の単位ベクトル
    Vector3 losUnit = Normalize(lineOfSight);
    
    // 相対速度から視線方向成分を除去
    Vector3 velocityPerp;
	velocityPerp.x = relativeVelocity.x - Dot(relativeVelocity, losUnit) * losUnit.x;
	velocityPerp.y = relativeVelocity.y - Dot(relativeVelocity, losUnit) * losUnit.y;
	velocityPerp.z = relativeVelocity.z - Dot(relativeVelocity, losUnit) * losUnit.z;
    
    // 回避行動のような動きを追加（曲線的な軌道用）
    float curveFactor = sinf(stateTimer_ * 0.1f) * 0.02f;
    Vector3 curveVector = Cross(losUnit, Vector3{0, 1, 0}) * curveFactor;
    
    // 操舵方向を計算
    Vector3 steeringDir = Normalize(losUnit + velocityPerp * 0.2f + curveVector);
    
    // 現在の速度方向
    Vector3 currentDir = Normalize(velocity_);
    
    // 操舵力を計算（現在の方向から目標方向への変化）
    Vector3 steeringForce = (steeringDir - currentDir) * navigationGain_;
    
    return steeringForce;
}

void PlayerBullet::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, 
                       PointLight pointLight, SpotLight spotLight) {
    // アクティブな場合のみ描画
    if (isActive_) {
        // 弾を進行方向に向ける
        if (Length(velocity_) > 0.01f) {
            Vector3 direction = Normalize(velocity_);
            // Y軸回転角を計算
            float rotY = atan2f(direction.x, direction.z);
            // X軸回転角を計算（上下の角度）
            float xzLength = sqrtf(direction.x * direction.x + direction.z * direction.z);
            float rotX = -atan2f(direction.y, xzLength);
            
            worldTransform_->transform.rotate = {rotX, rotY, 0.0f};
        }
       
    }
    // モデルの描画
    model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, 
        pointLight, spotLight);
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void PlayerBullet::OnCollisionEnter(BaseObject *other) {
    //========================================
    // 敵接触
    if (Enemy* enemy = dynamic_cast<Enemy*>(other)) {
        //---------------------------------------
        //弾を消す
        isActive_ = false;
    }


}
///--------------------------------------------------------------
///						接触継続処理
void PlayerBullet::OnCollisionStay(BaseObject *other) {

}

///--------------------------------------------------------------
///						接触終了処理
void PlayerBullet::OnCollisionExit(BaseObject *other) {
}