#pragma once
#include "BaseEffect.h"

// \brief GaussianFilter  
// ガウス関数に基づいて画像を滑らかにぼかすポストエフェクトクラス。  
// ボックスフィルターより自然なブラー効果を実現し、光のにじみや被写界深度表現などに使用される。

class GaussianFilter : public BaseEffect
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

