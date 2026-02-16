#pragma once

#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"	
#include "ViewProjection.h"	

#include "ICameraState.h"

// \brief BaseCamera
// カメラの基底クラス。
// ViewProjection行列の生成・保持・更新を提供し、派生カメラで共通処理を実装可能。

namespace Kouro
{
	class BaseCamera
	{
	public:

		/// \brief 初期化
		virtual void Initialize();

		/// \brief 更新
		virtual void Update();

		/**
		* \brief ViewProjection取得
		* \return ViewProjection
		*/
		virtual ViewProjection& GetViewProjection() { return *viewProjection_; }

		WorldTransform& GetWorldTransform() { return *worldTransform_; }

		/**
		* \brief  オフセットの取得
		* \return オフセット
		*/
		const Vector3 GetOffset() const { return offset_; }


		const Vector3 GetShakeOffset() const { return shakeOffset_; }


		void SetShakeOffset(Vector3 offset) { shakeOffset_ = offset; }

		/**
		* \brief オフセット設定
		* \param offset オフセット値
		*/
		void SetOffset(Vector3 offset) { offset_ = offset; }

		/**
		* \brief  状態の変更
		* \param  newState : 新しい状態
		*/
		void ChangeState(std::unique_ptr<ICameraState> newState);

		/**
		* \brief  親ワールド変換の設定
		* \param
		*/
		void SetParent(const WorldTransform* parent) { worldTransform_->SetParent(parent); }

		// \brief  回転を考慮したオフセットの計算
		Vector3 CalculationOffset();

	protected:

		std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

		std::unique_ptr<ViewProjection> viewProjection_ = nullptr;

		Matrix4x4 worldMatrix = MakeIdentity4x4();

		Vector3 shakeOffset_ = {};

		Vector3 offset_ = {};

		std::unique_ptr<ICameraState> state_ = nullptr;
	};
}