#pragma once
#include <algorithm>

#include "Input.h"
#include "BaseCamera.h"
#include "WorldTransform.h"

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

