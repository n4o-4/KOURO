#include "GpuResourceInternal.h"

namespace Kouro
{
	namespace GpuResourceInternal
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
		{
			// アップロード用バッファの作成
			Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
			D3D12_HEAP_PROPERTIES uploadHeapProperties{};
			uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
			D3D12_RESOURCE_DESC vertexBufferDesc{};
			vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			vertexBufferDesc.Width = sizeInBytes;
			vertexBufferDesc.Height = 1;
			vertexBufferDesc.DepthOrArraySize = 1;
			vertexBufferDesc.MipLevels = 1;
			vertexBufferDesc.SampleDesc.Count = 1;
			vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resultResource));
			return resultResource;
		}

		Microsoft::WRL::ComPtr<ID3D12Resource> CreateUploadBufferResource(ID3D12Device* device, size_t sizeInBytes)
		{
			// ヒーププロパティの設定
			D3D12_HEAP_PROPERTIES heapProperties{};
			heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

			// バッファのリソース記述の設定
			D3D12_RESOURCE_DESC bufferDesc{};
			bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			bufferDesc.Width = sizeInBytes;
			bufferDesc.Height = 1;
			bufferDesc.DepthOrArraySize = 1;
			bufferDesc.MipLevels = 1;
			bufferDesc.SampleDesc.Count = 1;
			bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			// アップロード用バッファの作成
			Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resultResource));

			return resultResource;
		}
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBufferResource(ID3D12Device* device, size_t sizeInBytes)
		{
			// ヒーププロパティの設定
			D3D12_HEAP_PROPERTIES heapProperties{};
			heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

			// バッファのリソース記述の設定
			D3D12_RESOURCE_DESC bufferDesc{};
			bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			bufferDesc.Width = sizeInBytes;
			bufferDesc.Height = 1;
			bufferDesc.DepthOrArraySize = 1;
			bufferDesc.MipLevels = 1;
			bufferDesc.SampleDesc.Count = 1;
			bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			// アップロード用バッファの作成
			Microsoft::WRL::ComPtr<ID3D12Resource> resultResource;
			device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resultResource));

			return resultResource;
		}
	}
}