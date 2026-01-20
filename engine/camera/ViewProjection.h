#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Structs.h"

#include "DirectXCommon.h"
#include "WorldTransform.h"

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
	void Update(const WorldTransform& worldTransform);

	/**
	* \brief  GetViewProjectionResource ビュー・プロジェクション行列のリソース取得
	* \return viewProjectionResource_ ビュー・プロジェクション行列のリソース
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> &GetViewProjectionResource() { return viewProjectionResource_; }

	/**
	* \brief  ワールド行列の取得
	* \return ワールド行列
	*/
	const Matrix4x4& GetWorldMatrix() const { return matWorld_; }

	/**
	* \brief  ビュー行列の取得
	* \return ビュー行列
	*/
	const Matrix4x4& GetViewMatrix() const { return matView_; }

	/**
	* \brief  プロジェクション行列の取得
	* \return プロジェクション行列
	*/
	const Matrix4x4& GetProjectionMatrix() const { return matProjection_; }

	/**
	* \brief  ワールド座標での位置を取得
	* \return worldPosition_ ワールド座標での位置
	*/
	const Vector3& GetWorldPosition() const { return worldPosition_; }

	/**
	* \brief  SetOffset オフセット設定
	* \param  offset オフセット値
	*/
	void SetOffset(Vector3 offset) { shakeOffset_ = offset; }

	/**
	* \brief  SetFovY 視野角設定
	* \param  fovY 視野角
	*/
	void SetFovY(float fovY) { fovY_ = fovY; }

	/**
	* \brief  SetAspectRation アスペクト比設定
	* \param  aspectRation アスペクト比
	*/
	void SetAspectRation(float aspectRation) { aspectRation_ = aspectRation; }

	/**
	* \brief  SetNearClip ニアクリップ設定
	* \param  nearClip ニアクリップ
	*/
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }

	/**
	* \brief  SetFarClip ファークリップ設定
	* \param  farClip ファークリップ
	*/
	void SetFarClip(float farClip) { farClip_ = farClip; }

	/// \brief 行列転送
	void TransferMatrix();

private:

	Vector3 worldPosition_;
	Matrix4x4 matWorld_;
	Matrix4x4 matView_;
	Matrix4x4 matProjection_;

	float fovY_;
	float aspectRation_;
	float nearClip_;
	float farClip_;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> viewProjectionResource_;

	ConstBufferDataViewProjection* viewProjectionData_;

	Vector3 shakeOffset_ = {};
};
