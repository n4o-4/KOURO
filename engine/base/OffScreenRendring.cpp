#include "OffScreenRendring.h"

#include "DirectXCommon.h"

void OffScreenRendring::Initialzie()
{
	CreateRenderTextureRTV();
}

void OffScreenRendring::PreDraw()
{
	// TransitionBarrierの設定
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	// NONEにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = renderTextureResources.Get();

	// 還移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// 還移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// TransitionBarrierを張る
	DirectXCommon::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);

	// DSV設定
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXCommon::GetInstance()->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	DirectXCommon::GetInstance()->GetCommandList()->OMSetRenderTargets(1, DirectXCommon::GetInstance()->GetRTVHandle(2), false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 1.0f,0.0f,0.0f,1.0f }; // 青っぽい色 RGBAの順
	DirectXCommon::GetInstance()->GetCommandList()->ClearRenderTargetView(*DirectXCommon::GetInstance()->GetRTVHandle(2), clearColor, 0, nullptr);

	// 画面全体の深度をクリア
	DirectXCommon::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	
	DirectXCommon::GetInstance()->GetCommandList()->RSSetViewports(1, DirectXCommon::GetInstance()->GetViewPort()); // Viewportを設定
	DirectXCommon::GetInstance()->GetCommandList()->RSSetScissorRects(1, DirectXCommon::GetInstance()->GetRect()); // Scissorを設定


}

void OffScreenRendring::PostDraw()
{
	// 還移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// 還移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// TransitionBarrierを張る
	DirectXCommon::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);


}

Microsoft::WRL::ComPtr<ID3D12Resource> OffScreenRendring::CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(width); // Textureの幅
	resourceDesc.Height = UINT(height); // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = format; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプルカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // Textureの次元数
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// 利用するHeapの設定。
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&resource));

	return resource;
}

void OffScreenRendring::CreateRenderTextureRTV()
{
	const Vector4 kRenderTargetClearValue = { 1.0f,0.0f,0.0f,1.0f };
	renderTextureResources = CreateRenderTextureResource(
		DirectXCommon::GetInstance()->GetDevice(),
		WinApp::kClientWidth,
		WinApp::kClientHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		kRenderTargetClearValue);

	DirectXCommon::GetInstance()->SetRTVHandle(2);
	DirectXCommon::GetInstance()->GetDevice()->CreateRenderTargetView(renderTextureResources.Get(), DirectXCommon::GetInstance()->GetRtvDesc(), *DirectXCommon::GetInstance()->GetRTVHandle(2));
}
