#include "TextureManager.h"

std::unique_ptr<TextureManager> TextureManager::instance = nullptr;

// ImGuiで0番目を使用するため、1晩から使用
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<TextureManager>();
	}

	return instance.get();
}

void TextureManager::Initialize(DirectXCommon* dxCommon,SrvManager* srvManager)
{
	// SRVの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
	
	dxCommon_ = dxCommon;

	srvManager_ = srvManager;

	CreateRenderTextureMetaData();
}

void TextureManager::Finalize()
{
	instance.reset();
}

void TextureManager::LoadTexture(const std::string& filePath)
{

	/*if (textureDatas.find(filePath) != textureDatas.end()) {
		return;
	}*/

	if (textureDatas.contains(filePath))
	{
		return;
	}

	

	assert(srvManager_->CheckSrvCount());

	// テクスチャ枚数上限チェック
	assert(textureDatas.size() + kSRVIndexTop < SrvManager::kMaxSRVCount);

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// テクスチャーデータを追加
	//textureDatas.resize(textureDatas.size() + 1);

	// テクスチャデータを追加して書き込む
	TextureData& textureData = textureDatas[filePath];

	// 追加したテクスチャデータの参照を取得する
	//TextureData& textureData = textureDatas.back();

	//textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metadata);

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素番号をSRVのインデックスとする
	/*uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxCommon_->GetCPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetGPUDescriptorHandle(dxCommon_->srvDescriptorHeap, dxCommon_->descriptorSizeSRV,srvIndex);*/

	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, textureData.metadata.mipLevels);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	 //読み込み済みテクスチャデータを検索
	/*if (textureDatas.find(filePath) != textureDatas.end()) {

		textureDatas.

	}


	if (textureDatas.contains(filePath)) {
		
	}*/

	auto it = textureDatas.find(filePath);

	if (it != textureDatas.end()) {
		return it->second.srvIndex;
	}

	//assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(std::string filePath)
{
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(std::string filePath)
{
	// TODO: return ステートメントをここに挿入します
	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;

}

void TextureManager::CreateRenderTextureMetaData()
{


	TextureData& textureData = textureDatas["RenderTexture"];

	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	srvManager_->CreateOffScreenTexture(textureData.srvIndex);
}
