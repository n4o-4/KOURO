#include "LockOnMarker.h"
#include <cmath> // 数学関数用

void LockOnMarker::Initialize() {
    // Multi lock on
    multiLockOn_ = std::make_unique<Object3d>();
    multiLockOn_->Initialize(Object3dCommon::GetInstance());
    ModelManager::GetInstance()->LoadModel("lockOn/Lock_on1.obj");
    multiLockOn_->SetModel("lockOn/Lock_on1.obj");

    multiLockOn_->SetEnableLighting(false);

	multilockOnWorldTransform_ = std::make_unique<WorldTransform>();
	multilockOnWorldTransform_->Initialize();
    
    // 最初は最大スケールから始める
    multilockOnWorldTransform_->transform.scale = { maxScale_, maxScale_, maxScale_ };

    isVisible_ = false;
    isAnimating_ = false;
    animationTimer_ = 0.0f;
}

void LockOnMarker::Show() {
    isVisible_ = true;
    isAnimating_ = true;
    // アニメーションを開始時は最大スケールから始める
    multilockOnWorldTransform_->transform.scale = { maxScale_, maxScale_, maxScale_ };
    animationTimer_ = 0.0f;
}

void LockOnMarker::Hide() {
    isVisible_ = false;
    isAnimating_ = false;
}

// プレイヤー位置とマーカー位置から向きを計算するUpdateメソッド
void LockOnMarker::Update(const Vector3& playerPosition, const Vector3& markerPosition) {
    if (!isVisible_) {
        return;
    }
    
    // マーカーの位置を設定
    multilockOnWorldTransform_->transform.translate = markerPosition;
    
    // プレイヤーからマーカーへの方向ベクトルを計算
    Vector3 toPlayer = {
        playerPosition.x - markerPosition.x,
        playerPosition.y - markerPosition.y,
        playerPosition.z - markerPosition.z
    };
    
    // ベクトルの長さを計算
    float length = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y + toPlayer.z * toPlayer.z);
    
    // 長さが0に近い場合は計算しない（ゼロ除算防止）
    if (length > 0.001f) {
        // 正規化
        Vector3 normalized = {
            toPlayer.x / length,
            toPlayer.y / length,
            toPlayer.z / length
        };
        
        // Y軸周りの回転角（水平方向）を計算
        float yaw = std::atan2(normalized.x, normalized.z);
        
        // X軸周りの回転角（垂直方向）を計算
        float pitch = std::atan2(-normalized.y, std::sqrt(normalized.x * normalized.x + normalized.z * normalized.z));
        
        // マーカーの回転を設定
        multilockOnWorldTransform_->transform.rotate = { pitch, yaw, 0.0f };
    }
    
    // アニメーション処理
    if (isAnimating_) {
        // タイマーを進める
        animationTimer_ += 1.0f / 60.0f; // 60FPSを想定
        
        // 線形に縮小するアニメーション
        float t = std::min(animationTimer_ * animationSpeed_, 1.0f);
        float currentScale = maxScale_ - (maxScale_ - minScale_) * t;
        
        // スケール値を設定
        multilockOnWorldTransform_->transform.scale = { 
            currentScale, 
            currentScale, 
            currentScale 
        };
    }
    
    // 行列を更新
    multilockOnWorldTransform_->UpdateMatrix();
    multiLockOn_->SetLocalMatrix(MakeIdentity4x4());
    multiLockOn_->Update();
}

void LockOnMarker::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	if (isVisible_) {
	multiLockOn_->Draw(*multilockOnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}
