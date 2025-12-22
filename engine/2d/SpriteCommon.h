#pragma once
#include "DirectXCommon.h"

// \brief SpriteCommon
// スプライト描画に必要な共通リソースやパイプラインを管理するクラス。
// ルートシグネチャやグラフィックスパイプライン（前景・背景）を生成・保持し、
// スプライト描画時に適切なパイプライン設定を提供。
// シングルトンとして管理され、DirectXCommonを介してDirectX12描画にアクセスする。

class SpriteCommon
{
public: // メンバ関数

	/**
	* \brief  インスタンス取得
	* \return インスタンス
	*/
	static SpriteCommon* GetInstance();

	/**
	* \brief  初期化
	* \param  dxCommon DirectX共通クラスへのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon);

	/// \brief 終了処理
	void Finalize();

	/// \brief 前景ビュー設定
	void SetForegroundView();

	/// \brief 背景ビュー設定
	void SetBackgroundView();

	/// \brief 前景描画
	void DrawForeground();

	/// \brief 背景描画
	void DrawBackground();

	/**
	* \brief  directXCommon取得
	* \return directXCommon
	*/
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/**
	* \brief  背景描画フラグ取得
	* \return 背景描画フラグ
	*/
	bool GetIsDrawBackground() const { return isDrawBackground; }

	/**
	* \brief  前景描画フラグ取得
	* \return 前景描画フラグ
	*/
	bool GetIsDrawForeground() const { return isDrawForeground; }	

private:

	static std::unique_ptr<SpriteCommon> instance;

	friend std::unique_ptr<SpriteCommon> std::make_unique<SpriteCommon>();
	friend std::default_delete<SpriteCommon>;

	SpriteCommon() = default;
	~SpriteCommon() = default;
	SpriteCommon(SpriteCommon&) = delete;
	SpriteCommon& operator=(SpriteCommon&) = delete;

	/// \brief ルートシグネチャ作成
	void CreateRootSignature();

	/// \brief グラフィックスパイプライン作成（前景）
	void CreateGraphicsPipelineForeground();

	/// \brief グラフィックスパイプライン作成（背景）
	void CreateGraphicsPipelineBackground();

private:
	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateBackground;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForeground;

	//ID3DBlob* signatureBlob = nullptr;
	Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	bool isDrawBackground = false;	

	bool isDrawForeground = false;	
};