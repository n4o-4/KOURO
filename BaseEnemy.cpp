#include "BaseEnemy.h"
#include "PlayerBullet.h"
#include <cmath>
#include <algorithm>

///=============================================================================
///						初期化
void BaseEnemy::Initialize() {
    //========================================
    // モデルを初期化
    model_ = std::make_unique<Object3d>();
    model_->Initialize(Object3dCommon::GetInstance());
    //========================================
    // モデルを読み込む
    ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
    model_->SetModel("enemy/enemy.obj");
    //========================================
    // 初期位置を設定
    worldTransform_ = std::make_unique<WorldTransform>();
    worldTransform_->Initialize();
    //========================================
    // 当たり判定との同期
    BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);

    // 乱数生成器の初期化
    std::random_device rd;
    rng_ = std::mt19937(rd());

    // スポーン位置の初期化
    spawnPosition_ = worldTransform_->transform.translate;
}

///=============================================================================
///						描画
void BaseEnemy::Update() {
    if (hp_ > 0) {
       
        // 弾の更新
        BulletUpdate();

        // ワールド変換の更新
        worldTransform_->UpdateMatrix();
        // モデルのローカル行列を更新
        model_->SetLocalMatrix(MakeIdentity4x4());
        // モデルの更新
        model_->Update();

        //========================================
        // 当たり判定との同期
        BaseObject::Update(worldTransform_->transform.translate);
    }
}

///=============================================================================
///						描画
void BaseEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BulletDraw(viewProjection, directionalLight, pointLight, spotLight);

    //========================================
    // モデルの描画
    if (hp_ > 0) {
        model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
    }
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void BaseEnemy::OnCollisionEnter(BaseObject* other) {
}

///--------------------------------------------------------------
///						接触継続処理
void BaseEnemy::OnCollisionStay(BaseObject* other) {
}

///--------------------------------------------------------------
///						接触終了処理
void BaseEnemy::OnCollisionExit(BaseObject* other) {
}

void BaseEnemy::BulletUpdate() {
}

void BaseEnemy::BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
        ++it;
    }
}

void BaseEnemy::Fire() {
    if (target_) {
        std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
        newBullet->Initialize(*worldTransform_.get(), target_->transform.translate);
        bullets_.push_back(std::move(newBullet));
    }
}
// コンテキストベースの方向選択
Vector3 BaseEnemy::SelectDirection() {
    // 実装は省略（シンプルさを優先）
    return velocity_;
}