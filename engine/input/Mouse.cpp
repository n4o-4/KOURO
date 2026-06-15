#include "Mouse.h"

#include "WinApp.h"

void Kouro::Mouse::Initialize(IDirectInput8* directInput, WinApp* winApp)
{
	assert(directInput);
	assert(winApp);

	HRESULT result;
	// マウスデバイス生成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse_, nullptr);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = mouse_->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void Kouro::Mouse::Update()
{
	// 前回のマウス状態を保存
	previousMouseState_ = currentMouseState_;
	HRESULT result;

	// デバイスがフォーカスを失っている場合は再取得
	result = mouse_->Acquire();
	if (FAILED(result)) {
		return; // デバイスが使えないなら更新しない
	}

	// 入力状態を取得
	result = mouse_->GetDeviceState(sizeof(DIMOUSESTATE2), &currentMouseState_);
	if (FAILED(result)) {
		// 一部の環境ではここで E_INPUTLOST や DIERR_NOTACQUIRED が返る
		ZeroMemory(&currentMouseState_, sizeof(currentMouseState_)); // 安全対策
		return;
	}

	// マウス座標の取得
	//GetCursorPos(&mousePos);
	//ScreenToClient(winApp->GetHWND(), &mousePos);
}

bool Kouro::Mouse::PushButton(MouseButton mouseButton)
{
	// 指定されたマウスボタンが押されているかチェック
	return (currentMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) != 0;
}

bool Kouro::Mouse::TriggerButton(MouseButton mouseButton)
{
	// 指定されたマウスボタンが押された瞬間かチェック
	return (currentMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) != 0 &&
		!(previousMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80);
}
