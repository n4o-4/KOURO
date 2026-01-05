#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"

#include "MyMath.h"

constexpr float kDefaultCameraDistance = -15.0f;
constexpr float kDefaultFovY = 0.85f; //!< 視野角（約49°）
constexpr float kDefaultNearClip = 0.1f;
constexpr float kDefaultFarClip = 100.0f;

//struct ConstBufferDataViewProjection
//{
//	Vector3 worldPosition;
//	float padding[1];
//	Matrix4x4 matView;
//	Matrix4x4 matProjection;
//};

struct ConstBufferDataViewProjection
{
	Vector3 worldPosition;
	float padding[1];
	Matrix4x4 matViewProjection;
};

// \brief ViewProjection
// ビュー行列とプロジェクション行列を管理するクラス。
// カメラの位置・回転・投影設定を保持し、GPU用の定数バッファも管理する。

class ViewProjection
{
public:

	/// \brief  Initialize 初期化
	void Initialize();

	/// \brief  Update 更新
	void Update();

	/**
	* \brief  GetViewProjectionResource ビュー・プロジェクション行列のリソース取得
	* \return viewProjectionResource_ ビュー・プロジェクション行列のリソース
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> &GetViewProjectionResource() { return viewProjectionResource_; }

	/**
	* \brief  SetOffset オフセット設定
	* \param  offset オフセット値
	*/
	void SetOffset(Vector3 offset) { traslateOffset = offset; }

public:

	Transform transform;

	Vector3 worldPosition_;
	Matrix4x4 matWorld_;
	Matrix4x4 matView_;
	Matrix4x4 matProjection_;

	float fovY;
	float aspectRation;
	float nearClip;
	float farClip;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> viewProjectionResource_;

	ConstBufferDataViewProjection* viewProjectionData_;

	Vector3 traslateOffset = {};
};
