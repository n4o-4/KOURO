#pragma once
#include <algorithm>

#include "Input.h"
#include "BaseCamera.h"
#include "WorldTransform.h"

//constexpr float kDefaultCameraDistance = -10.0f;
constexpr float kScrollSensitivity = 0.01f; //!< ホイール感度
constexpr float kMinZoomDistance = -100000.0f; //!< ズームアウトの下限距離
constexpr float kMaxZoomDistance = -1.0f;      //!< ズームインの上限距離
constexpr float kMouseRotateSensitivity = 0.0008f;

// \brief DebugCamera
// デバッグ用カメラ。自由に移動・回転が可能で、ターゲットの位置に応じた追従もできる。

class DebugCamera : public BaseCamera
{
public:

	/// \brief 初期化
	void Initialize() override;

	/// \brief 更新
	void Update() override;

	/**
	* \brief ターゲット設定
	* \param target ターゲットのワールド変換のポインタ
	*/
	ViewProjection& GetViewProjection() { return *viewProjection_; }

private:
		
	std::unique_ptr<WorldTransform> targetTransform_ = nullptr;

	// ターゲットからのオフセット 
	Vector3 offset = { 0.0f,0.0f,-10.0f };
};

