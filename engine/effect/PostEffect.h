#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "CameraManager.h"

class PostEffect
{
public:
	//==============================
	// エフェクトの種類
	enum class EffectType
	{
		// None
		None,

		// モノクロ
		Grayscale,

		// 周辺減光効果
		Vignette,

		// ボックスフィルターによるぼかし
		BoxFilter,

		// ガウス関数によるぼかし
		GaussianFilter,

		// 輝度検出でのアウトライン
		LuminenceBasedOutline,

		// 深度検出でのアウトライン
		DepthBasedOutline,

		// RadialBlur
		RadialBlur,

		// ↑↑↑追加↑↑↑

		EffectCount,
	};

	//==============================
	// 深度リソース用の構造体
	struct Material
	{
		Matrix4x4 projectionInverse;
	};

private:

	struct Pipeline
	{
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct Effect
	{
		std::list<std::unique_ptr<Pipeline>> pipelines_;
		bool isActive[static_cast<int>(EffectType::EffectCount)];
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

	void CreatePipeLine();

	// None
	void NormalRootSignature(Pipeline* pipeline);
	void NormalPipeLine(Pipeline* pipeline);

	// グレイスケール
	void GrayscaleRootSignature(Pipeline* pipeline);
	void GrayscalePipeLine(Pipeline* pipeline);

	// 周辺減光効果
	void VignetteRootSignature(Pipeline* pipeline);
	void VignettePipeLine(Pipeline* pipeline);

	// ボックスフィルターによるぼかし
	void BoxFilterRootSignature(Pipeline* pipeline);
	void BoxFilterPipeLine(Pipeline* pipeline);

	// ガウス関数によるぼかし
	void GaussianFilterRootSignature(Pipeline* pipeline);
	void GaussianFilterPipeLine(Pipeline* pipeline);

	// 輝度検出でのアウトライン
	void LuminanceBasedOutlineRootSignature(Pipeline* pipeline);
	void LuminanceBasedOutlinePipeLine(Pipeline* pipeline);

	// 深度検出でのアウトライン
	void DepthBasedOutlineRootSignature(Pipeline* pipeline);
	void DepthBasedOutlinePipeLine(Pipeline* pipeline);

	// RadialBlur
	void RadialBlurRootSignature(Pipeline* pipeline);
	void RadialBlurPipeLine(Pipeline* pipeline);

private: // メンバ変数
	
	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	Effect effect;

	CameraManager* cameraManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Material* materialData_ = nullptr;
};

