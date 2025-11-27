#pragma once
#include "BaseEffect.h"

// \brief BoxFilter
// 画面全体にボックスフィルターを適用してブラー効果を実現するポストエフェクトクラス。  
// パイプライン構築と描画処理を担当し、描画結果を柔らかくぼかす。

class BoxFilter : public BaseEffect
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

private:

	void CreatePipeline();


	void CreateRootSignature(Pipeline* pipeline);


	void CreatePipeLineState(Pipeline* pipeline);


	void DrawImGui() override;
};

