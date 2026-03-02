#include "GpuResourceInternal.h"

namespace Kouro
{
	namespace GpuResourceInternal
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
		{
			// アップロード用バッファの作成
			D3D12_HEAP_PROPERTIES uploadHeapProperties{};
			uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
			D3D12_RESOURCE_DESC resourceDesc{};
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resourceDesc.Width = sizeInBytes;
			resourceDesc.Height = 1;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.MipLevels = 1;
			resourceDesc.SampleDesc.Count = 1;
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			// リソースの生成
			Microsoft::WRL::ComPtr<ID3D12Resource> rResource;
			HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rResource));
			return rResource;
		}

		Microsoft::WRL::ComPtr<ID3D12Resource> Create2DTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
		{
			// 利用するHeapの設定。
			D3D12_HEAP_PROPERTIES heapProperties{};
			heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

			// metadataを基にResourceの設定
			D3D12_RESOURCE_DESC resourceDesc{};
			resourceDesc.Width = UINT(metadata.width); // Textureの幅
			resourceDesc.Height = UINT(metadata.height); // Textureの高さ
			resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
			resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列Textureの配列数
			resourceDesc.Format = metadata.format; // TextureのFormat
			resourceDesc.SampleDesc.Count = 1; // サンプルカウント。1固定
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数

			// Resourceの生成
			Microsoft::WRL::ComPtr<ID3D12Resource> rResource = nullptr;
			HRESULT hr = device->CreateCommittedResource(&heapProperties,D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_COPY_DEST,nullptr,IID_PPV_ARGS(&rResource));
			assert(SUCCEEDED(hr));
			return rResource;
		}
	}
}