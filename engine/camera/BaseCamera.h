#pragma once

#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"	
#include "ViewProjection.h"	

// \brief BaseCamera
// カメラの基底クラス。
// ViewProjection行列の生成・保持・更新を提供し、派生カメラで共通処理を実装可能。

class BaseCamera
{
public:

	/// <summary>
	/// 初期化
	/// viewProjectionの生成と初期化
	/// </summary>
	
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// viewProjectionの更新	
	/// </summary>
	
	virtual void Update();

	virtual ViewProjection& GetViewProjection() { return *viewProjection_; }

	void SetOffset(Vector3 offset) { offset_ = offset; }

protected:

	std::unique_ptr<ViewProjection> viewProjection_ = nullptr;

	Matrix4x4 worldMatrix = MakeIdentity4x4();

	Vector3 offset_ = {};

};