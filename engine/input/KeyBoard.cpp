#include "KeyBoard.h"

#include <cstring>

#include "WinApp.h"


void Kouro::KeyBoard::Initialize(IDirectInput8* directInput, WinApp* winApp)
{
	assert(directInput);
	assert(winApp);

	HRESULT result;

	// キーボードデバイス生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Kouro::KeyBoard::Update()
{
	// 前回のキー入力を保存
	std::memcpy(previousKeys_, currentKeys_, sizeof(currentKeys_));

	// キーボード情報取得
	keyboard_->Acquire();

	// 入力状態を取得
	HRESULT result = keyboard_->GetDeviceState(sizeof(currentKeys_), currentKeys_);
	if (FAILED(result))
	{
		std::memset(currentKeys_, 0, sizeof(currentKeys_));
	}
}

bool Kouro::KeyBoard::PushKey(BYTE keyNumber)
{
	// 指定されたキーが押されているかチェック
	return (currentKeys_[keyNumber] & 0x80) != 0;
}

bool Kouro::KeyBoard::TriggerKey(BYTE keyNumber)
{
	// 指定されたキーが押された瞬間かチェック
	return (currentKeys_[keyNumber] & 0x80) != 0 && !(previousKeys_[keyNumber] & 0x80);
}