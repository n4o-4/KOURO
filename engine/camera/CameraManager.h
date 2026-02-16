#pragma once
#include "BaseCamera.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "RailCamera.h"
#include <random>



// \brief CameraManager
// 複数カメラを管理するクラス。
// デバッグカメラや追従カメラなどを保持し、更新・切り替え・ターゲット設定を行う。

namespace Kouro
{
	constexpr float kShakeAmplitude = 0.2f; //!< カメラシェイク最大振幅


	class CameraManager
	{
	public: /// メンバ関数	

		/// \brief  Initialize 初期化
		void Initialize();

		/// \brief  Update 更新
		void Update();

		/**
		* \brief  GetActiveCamera アクティブなカメラの取得
		* \return activeCamera_ アクティブなカメラのポインタ
		*/
		BaseCamera* GetActiveCamera() { return activeCamera_; }

		/**
		* \brief  SetFollowCameraTarget 追従カメラのターゲット設定
		* \param  target ターゲットのワールド変換のポインタ
		*/
		void SetFollowCameraTarget(WorldTransform* target) { followCamera_->SetTarget(target); }

		/**
		* \brief  GetFollowCamera 追従カメラの取得
		* \return followCamera_ 追従カメラのポインタ
		*/
		FollowCamera* GetFollowCamera() { return followCamera_.get(); }

		/**
		* \brief  SetActiveCamera アクティブなカメラの設定
		* \param  camera アクティブにするカメラのポインタ
		*/
		void SetActiveCamera(BaseCamera* camera) { activeCamera_ = camera; }

		/// \brief  CamerasClear カメラのクリア
		void CamerasClear();

		/**
		* \brief  CameraShake カメラシェイクの開始
		* \param  time シェイク時間
		*/
		void CameraShake(float time);

	public:

		/// \brief  Shake カメラシェイク処理
		void Shake();

		/// \brief  DrawDebugUI デバッグUIの描画
		void DrawDebugUI();

	private:

		std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

		std::unique_ptr<FollowCamera> followCamera_ = nullptr;

		std::list<BaseCamera*> cameras_;

		BaseCamera* activeCamera_ = nullptr;

		bool isShaking = false;

		float shakeTime = 0.0f;

		float timer = 0.0f;

		std::mt19937 randomEngine;

		Vector3 shakeOffSet = {};
	};
}