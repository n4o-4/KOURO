#pragma once
#include "DirectXCommon.h"
#include "Camera.h"

// \brief Object3dCommon
// 3Dオブジェクト描画に共通で必要なリソースや設定を管理するクラス。
// DirectX12のルートシグネチャやパイプラインステート、デフォルトカメラ、環境マップなどを保持。
// シングルトンとして扱い、すべてのObject3dインスタンスで共有される。

class Object3dCommon
{
public: // メンバ関数

	/// \brief インスタンス取得
	static Object3dCommon* GetInstance();

	/**
	* \brief  初期化
	* \param  dxCommon DirectX共通クラスへのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon);

	/// \brief 終了処理
	void Finalize();

	/// \brief ビュー行列設定
	void SetView();

	/**
	* \brief  directXCommon取得
	* \return directXCommon
	*/
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	/**
	* \brief  デフォルトカメラ取得
	* \return デフォルトカメラ
	*/
	Camera* GetDefaultCamera() const { return defaultCamera_.get(); }

	/**
	* \brief  環境マップパスを設定する
	* \param  path 環境マップパス
	*/
	void SetEnvironmentMapPath(const std::string& path) { environmentMapPath_ = path; }

private:

	static std::unique_ptr<Object3dCommon> instance;

	friend std::unique_ptr<Object3dCommon> std::make_unique<Object3dCommon>();
	friend std::default_delete<Object3dCommon>;

	Object3dCommon() = default;
	~Object3dCommon() = default;
	Object3dCommon(Object3dCommon&) = delete;
	Object3dCommon& operator=(Object3dCommon&) = delete;

	/// \brief グラフィックスパイプライン作成
	void CreateGraphicsPipeline();

	/// \brief ルートシグネチャ作成
	void CreateRootSignature();

private: // メンバ変数
	DirectXCommon* dxCommon_;

    Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	//ID3DBlob* signatureBlob = nullptr;
	Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	std::unique_ptr<Camera> defaultCamera_ = nullptr;

	std::string environmentMapPath_;
};

