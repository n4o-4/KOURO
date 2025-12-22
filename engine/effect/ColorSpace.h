#pragma once
#include "BaseEffect.h"

namespace ColorSpaceShader
{
	struct Material
	{
		float hue;
		float saturation;
		float value;
		float padding[1];
	};
}

// \brief ColorSpace  
// HSV（色相・彩度・明度）を操作して画面全体の色調を変化させるポストエフェクトクラス。  
// パイプライン生成とマテリアル制御を行い、色味調整やトーン補正などの効果を適用する。

class ColorSpace : public BaseEffect
{
public: // 公開メンバ関数

	/**
	* \brief	初期化
	* \param	dxCommon DirectXCommonのポインタ
	* \param	srvManager SrvManagerのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	/// \brief  更新
	void Update() override;

	/**
	* \brief  描画
	* \param  renderTargetIndex レンダーターゲットのインデックス
	* \param  renderResourceIndex レンダーリソースのインデックス
	*/
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

	/// \brief  リセット
	void Reset() override { resource_.Reset(); }

private: // 非公開メンバ関数

	/// \brief  CreatePipeline パイプラインの作成
	void CreatePipeline();

	/**
	* \brief  CreateRootSignature ルートシグネチャの作成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreateRootSignature(Pipeline* pipeline);

	/**
	* \brief  CreatePipeLineState パイプラインステートの作成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreatePipeLineState(Pipeline* pipeline);

	/// \brief  CreateMaterial マテリアルの作成
	void CreateMaterial();

	/// \brief  DrawImGui ImGui描画
	void DrawImGui() override;

public:

	float hue = 0.0f;
	float saturation = 0.0f;
	float value = 0.0f;	

private: // メンバ変数

	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// ポインター
	ColorSpaceShader::Material* data_ = nullptr;

};

