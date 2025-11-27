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

	// シングルトンインスタンスの取得
	static Object3dCommon* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	// 共通描画設定
	void SetView();

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	//void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }

	Camera* GetDefaultCamera() const { return defaultCamera_.get(); }

	void SetEnvironmentMapPath(const std::string& path) { environmentMapPath_ = path; }

private:

	static std::unique_ptr<Object3dCommon> instance;

	friend std::unique_ptr<Object3dCommon> std::make_unique<Object3dCommon>();
	friend std::default_delete<Object3dCommon>;

	Object3dCommon() = default;
	~Object3dCommon() = default;
	Object3dCommon(Object3dCommon&) = delete;
	Object3dCommon& operator=(Object3dCommon&) = delete;

	// グラフィックスパイプラインの生成
	void CreateGraphicsPipeline();

	// ルートシグネチャの生成
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

