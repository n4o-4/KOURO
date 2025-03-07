#pragma once
#include "Kouro.h"

class LockOnMarker {

public:
    void Initialize();
    void Show();
    void Hide();
    // プレイヤー位置を受け取るようにUpdateメソッドを修正
    void Update(const Vector3& playerPosition, const Vector3& markerPosition);
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

    void SetTargetPosition(const Vector3& position) { multilockOnWorldTransform_->transform.translate = position; }

private:
    // Multi lock on
    std::unique_ptr<Object3d> multiLockOn_ = nullptr;
    std::unique_ptr<WorldTransform> multilockOnWorldTransform_ = nullptr;
    bool isVisible_ = false;

        // アニメーション用変数
        float animationTimer_ = 0.0f;    // アニメーションタイマー
        float maxScale_ = 3.0f;          // 最大スケール値
        float minScale_ = 1.5f;          // 最小スケール値 
        float animationSpeed_ = 2.0f;    // アニメーション速度
        bool isAnimating_ = false;       // アニメーション状態
};

