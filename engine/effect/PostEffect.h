#pragma once
#include <random>

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "CameraManager.h"

///==============================
// エフェクト
#include "BaseEffect.h"
#include "Grayscale.h"
#include "Vignette.h"
#include "BoxFilter.h"
#include "GaussianFilter.h"
#include "LuminanceBasedOutline.h"
#include "DepthBasedOutline.h"
#include "RadialBlur.h"
#include "Dissolve.h"
#include "Random.h"
#include "LinearFog.h"

class PostEffect
{
public:
	//==============================
	// エフェクトの種類
	enum class EffectType
	{
		// モノクロ
		Grayscale,

		// 周辺減光効果
		Vignette,

		// ボックスフィルターによるぼかし
		BoxFilter,

		// ガウス関数によるぼかし
		GaussianFilter,

		// 輝度検出でのアウトライン
		LuminanceBasedOutline,

		// 深度検出でのアウトライン
		DepthBasedOutline,

		// RadialBlur
		RadialBlur,

		// Dissolve
		Dissolve,

		// Random
		Random,

		// LinearFog
		LinearFog,

		// ↑↑↑追加↑↑↑

		EffectCount,
	};

private:

	struct Pipeline
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct ActiveEffect
	{
		std::unique_ptr<BaseEffect> effect;
		EffectType type;
	};

public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 指定のエフェクトを適応
	void ApplyEffect(EffectType type);

	// カメラマネージャを設定
	void SetCameraManager(CameraManager* cameraManager) { cameraManager_ = cameraManager; }
private:

private: // メンバ変数
	
	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	CameraManager* cameraManager_ = nullptr;

	std::list<std::unique_ptr<ActiveEffect>> activeEffects_;
};

