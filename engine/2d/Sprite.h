#pragma once
#include "wrl.h"
#include "d3d12.h"
#include "Vectors.h"
#include "MyMath.h"
#include <cstdint>
#include <string>

class SpriteCommon;

// \brief Sprite
// 2Dスプライト描画を管理するクラス。
// テクスチャ、位置、回転、スケール、アンカーポイント、色、左右・上下反転などを設定可能。
// 内部で頂点バッファ、インデックスバッファ、マテリアル、変換行列を管理し、DirectX12を用いた描画を行う。
// SpriteCommonを介して共通リソースを利用。

class Sprite
{
public: // メンバ関数

	/**
	* \brief  Initialize 初期化
	* \param  spriteCommon SpriteCommonのポインタ
	* \param  textureFilePath textureのファイルパス
	*/
	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);

	// \brief Update 更新
	void Update();

	// \brief Draw 描画
	void Draw();

	/**
	* \brief  GetPosition スプライトの位置を取得する
	* \return position スプライトの位置
	*/
	const Vector2& GetPosition() const { return position;}

	/**
	* \brief  GetRotation スプライトの回転を取得する
	* /return rotation スプライトの回転
	*/
	float GetRotation() const { return rotation; }

	/**
	* \brief  GetColor スプライトの色を取得する
	* \return color スプライトの色
	*/
	const Vector4 GetColor() const { return materialData->color; }

	/**
	* \brief  GetSize	描画サイズを取得する
	* \return size 描画サイズ
	*/
	const Vector2& GetSize() const { return size; }

	/**
	* \brief GetAnchorPoint アンカーポイントを取得する
	*/
	const Vector2& GetAnchorPoint() const { return anchorPoint; }

	/**
    * \brief GetIsFlipX X方向の反転フラグを取得する
    * \param isFlipX X方向の反転フラグ
    */
	const bool GetIsFlipX() { return isFlipX_; }

	/**
	* \brief GetIsFlipY Y方向の反転フラグを取得する
	* \param isFlipY Y方向の反転フラグ
	*/
	const bool GetIsFlipY() { return isFlipY_; }

	/**
	* \brief  GetTexLeftTop スプライト上の左上切り出し位置を取得する
	* \param textureLeftTop スプライト上の左上切り出し位置
	*/
	const Vector2& GetTexLeftTop() { return textureLeftTop; }

	/**
	* \brief  GetTexSize　テクスチャの切り出しサイズを取得する
	* \return textureSize テクスチャの切り出しサイズ
	*/
	const Vector2& GetTexSize() { return textureSize; }

	void SetPosition(const Vector2& position) { this->position = position; }
	
	void SetRotation(float rotation) { this->rotation = rotation; }

	void SetColor(const Vector4& color) { materialData->color = color; }

	void SetSize(const Vector2& size) { this->size = size; }

	// getter
	

	// setter
	void SetAnchorPoint(const Vector2& anchorPoint) { this->anchorPoint = anchorPoint; }

	

	// setter
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }

	

	// setter
	void SetTexLeftTop(const Vector2& leftTop) { textureLeftTop = leftTop; }

	/*
	 * \brief  SetTexSize テクスチャの切り出しサイズを設定
	*/
	void SetTexSize(const Vector2& TexSize) { textureSize = TexSize; }

private: 

	void CreateVertexData();

	void CreateIndexData();

	void CreateMaterialData();

	void CreateTransformationMatrixData();

	// 
	void AdjustTextureSize();

private:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material {
		Vector4 color;
		int enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	SpriteCommon* spriteCommon = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource = nullptr;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Material* materialData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource = nullptr;

	TransformationMatrix* transformationMatrixData;

	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };

	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Transform uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	std::string textureFilePath;

	Vector2 position = { 0.0f,0.0f };

	float rotation = 0.0f;

	Vector2 size = { 640.0f,360.0f };

	// テクスチャ番号
	uint32_t textureIndex = 0;

	Vector2 anchorPoint = { 0.0f,0.0f };

	// 左右フリップ
	bool isFlipX_ = false;

	// 上下フリップ
	bool isFlipY_ = false;

	// テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };

	// テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };

};

