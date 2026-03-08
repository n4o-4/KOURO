#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "ShaderManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3dCommon.h"
#include "ModelCommon.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"

#include "LineDrawerBase.h"
#include "PostEffect.h"

#include "LeakChecker.h"

#include "GpuParticle.h"
#include "GpuParticleManager.h"

#include "EngineContext.h"

#include <wrl.h>
#include <deque>

// \brief FrameWork ゲーム全体の基本的な流れ（初期化・更新・描画・終了）を管理するフレームワーククラス。DirectX環境や各種マネージャを統合し、アプリのメインループを制御する。 

namespace Kouro
{
	class Framework
	{
	public:

		virtual~Framework() = default;

		/// \brief 初期化
		virtual void Initialize();

		/// \brief 終了
		virtual void Finalize();

		/// \brief 更新
		virtual void Update();

		/// \brief 描画
		virtual void Draw();

		/// \brief エフェクト描画
		virtual void DrawEffect();

		/**
		* \brief  終了要求が来ているかどうか取得
		* \return endRequest_ 終了要求が来ているかどうか
		*/
		virtual bool IsEndRequest() { return endRequest_; }

		/// \brief Run メインループ
		void Run();

		/**
		* \brief  デルタタイムの取得する
		* \return kDeltaTime_ デルタタイム
		*/
		const float GetDeltaTime() const { return kDeltaTime_; }

		/// \brief GpuContextの作成
		GpuContext CreateGpuContext();

		/// \brief EngineContextの作成
		EngineContext CreateEngineContext();

	protected:
		
		/**
		* \brief  SrvManagerの取得
		* \return srvManager_ SrvManagerのポインタ
		*/
		SrvManager* GetSrvManager() { return srvManager_.get(); }

		/**
		* \brief  GpuParticleManagerの取得
		* \return gpuParticleManager_ GpuParticleManagerのポインタ
		*/
		GpuParticleManager* GetGpuParticleManager() { return gpuParticleManager_.get(); }

		/**
		* \brief  PostEffectの取得
		* \return postEffect_ PostEffectのポインタ
		*/
		PostEffect* GetPostEffect() { return postEffect_.get(); }

#ifdef _DEBUG
		/**
		* \brief  ImGuiManagerの取得
		* \return imGuiManager_ ImGuiManagerのポインタ。デバッグビルドでない場合はnullptr
		*/
		ImGuiManager* GetImGuiManager() { return imGuiManager_.get(); }
#endif

	private:

		void UpdateFPS();

		void CompileShaders();

	private:

		std::unique_ptr<WinApp> winApp_ = nullptr;

		ShaderManager shaderManager_;

		DirectXCommon* dxCommon_ = nullptr;

		std::unique_ptr<SrvManager> srvManager_ = nullptr;

		std::unique_ptr<ModelCommon> modelCommon_ = nullptr;

		std::unique_ptr<UavManager> uavManager_ = nullptr;

		const GpuResourceUtils* gpuResourceUtils_ = nullptr;

		EngineContext engineContext_;

#ifdef _DEBUG

		std::unique_ptr<ImGuiManager> imGuiManager_ = nullptr;

#endif
		std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

		std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;

		std::unique_ptr<PostEffect> postEffect_ = nullptr;

		std::unique_ptr<GpuParticleManager> gpuParticleManager_ = nullptr;

		GpuParticle* gpuParticle_ = nullptr;

		bool endRequest_ = false;


		/// \brief 時間計測用のメンバ変数
		std::chrono::steady_clock::time_point startTime_;     // 実行開始時間

		std::chrono::duration<float> elapsedTime_;            // 実行開始からの経過時間

		std::chrono::steady_clock::time_point prevTime_;      // 前回のフレームの時間

		std::chrono::steady_clock::time_point now_;           // 現在の時間

		std::chrono::duration<float> deltaTime_;              // 前フレームからの経過時間

		std::deque<std::chrono::duration<float>> deltaTimes_; // 過去数フレームのデルタタイムを保存するキュー

		UINT currentFrameCount_ = 0;                          // 実行開始からの累計フレーム数

		float frameHistoryDuration_ = 1.0f;                    // 過去何秒分のフレームを保持するか

		const float kDeltaTime_ = 1.0f / 60.0f;
	};
}