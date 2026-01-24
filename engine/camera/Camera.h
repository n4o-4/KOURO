#pragma once
#include "BaseCamera.h"

// \brief Camera
// シングルトン実装のカメラクラス。
// BaseCameraを継承し、回転・位置・FOV・アスペクト比を操作可能。
// ゲーム内で共通カメラとして使用する。

namespace Kouro
{
	class Camera : public BaseCamera
	{
	private:

		static std::unique_ptr<Camera> instance;

		friend std::unique_ptr<Camera> std::make_unique<Camera>();
		friend std::default_delete<Camera>;

		Camera() = default;
		~Camera() = default;
		Camera(Camera&) = delete;
		Camera& operator=(Camera&) = delete;


	public:
		/// \brief インスタンス取得
		static Camera* GetInstance();

		/// \brief 初期化
		void Initialize();

		/// \brief 終了処理
		void Finalize();

		/// \brief 更新
		void Update();

		/**
		* \brief 回転設定
		* \param rotate 回転値
		*/
		void SetRotate(const Vector3& rotate) { worldTransform_->transform.rotate = rotate; }

		/**
		* \brief 位置設定
		* \param translate 位置値
		*/
		void SetTranslate(const Vector3& translate) { worldTransform_->transform.translate = translate; }

		/**
		* \brief FOV設定
		* \param fovY FOV値
		*/
		void SetFovY(float fovY) { viewProjection_->SetFovY(fovY); }

		/**
		* \brief アスペクト比設定
		* \param aspectRation アスペクト比値
		*/
		void SetAspectRation(float aspectRation) { viewProjection_->SetAspectRation(aspectRation); }

		/**
		* \brief ViewProjection設定
		* \param viewProjection ViewProjection値
		*/
		void SetViewProjection(const ViewProjection& viewProjection) { *viewProjection_ = viewProjection; }

		/**
		* \brief ワールド行列取得
		* \return ワールド行列
		*/
		const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }

		/**
		* \brief 回転取得
		* \return 回転値
		*/
		const Vector3& GetRotate() const { return worldTransform_->GetRotate(); }

		/**
		* \brief 位置取得
		* \return 位置値
		* \note 位置値はViewProjectionのtransformから取得する
		*/
		const Vector3& GetTranslate() const { return worldTransform_->GetTranslate(); }

		/**
		* \brief ViewProjection取得
		* \return ViewProjection
		*/
		ViewProjection& GetViewProjection() { return *viewProjection_; }

	private:
	};
}