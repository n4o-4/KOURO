#include "SpriteRenderer.h"

void Kouro::SpriteRenderer::Initialize(ID3D12GraphicsCommandList* cmdList,const GpuResourceUtils* gpuResourceUtils)
{
	// 引数で受け取ってメンバ変数に記録する
	cmdList_ = cmdList;

	gpuResourceUtils_ = gpuResourceUtils;

	// 頂点バッファとインデックスバッファを作成する関数を呼び出す
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void Kouro::SpriteRenderer::Render(BaseScene* scene)
{
	cmdList_->SetGraphicsRootSignature(rootSignature.Get());

	cmdList_->SetPipelineState(graphicsPipelineStateForeground.Get());  // PSOを設定

	// 形状を設定
	cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファとインデックスバッファを設定
	cmdList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	cmdList_->IASetIndexBuffer(&indexBufferView_);
}

void Kouro::SpriteRenderer::CreateVertexBuffer()
{
	// 頂点バッファリソースを作成
	vertexResource_ = gpuResourceUtils_->CreateBufferResource(sizeof(Sprite::VertexData) * kVertexCount_);
	vertexData_ = nullptr;
	// 頂点バッファリソースにデータを書き込む
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点4つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Sprite::VertexData) * kVertexCount_;

	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Sprite::VertexData);
}

void Kouro::SpriteRenderer::CreateIndexBuffer()
{
	// インデックスバッファリソースを作成
	indexResource_ = gpuResourceUtils_->CreateBufferResource(sizeof(uint32_t) * kIndexCount_);
	indexData_ = nullptr;
	// インデックスバッファリソースにデータを書き込む
	indexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	// リソースの先頭アドレスから使う
	indexBufferView_.BufferLocation = indexResource_.Get()->GetGPUVirtualAddress();

	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexCount_;

	// インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// インデックスデータを設定
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
}