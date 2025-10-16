#pragma once

#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"	
#include "ViewProjection.h"	

/// <summary>
/// カメラの基底クラス
/// カメラの種類ごとに継承して使用する
/// ViewProjectionの生成と初期化、更新を行う
/// </summary>

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

protected:

	std::unique_ptr<ViewProjection> viewProjection_ = nullptr;

	Matrix4x4 worldMatrix = MakeIdentity4x4();

};