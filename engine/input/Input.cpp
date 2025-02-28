#include "Input.h"

std::unique_ptr<Input> Input::instance = nullptr;

Input* Input::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<Input>();
	}

	return instance.get();
}

void Input::Initialize(WinApp* winApp)
{
	this->winApp = winApp;

	HRESULT result;

	// DirectInputのインスタンス生成
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイス生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// マウスデバイスの作成
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, nullptr);
	assert(SUCCEEDED(result));

	result = mouse->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));

	result = mouse->SetCooperativeLevel(winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	result = mouse->Acquire();
	assert(SUCCEEDED(result));
}

void Input::Finalize()
{

}

void Input::Update()
{
	HRESULT result;

	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	// キーボード情報取得
	result = keyboard->Acquire();

	// 全キーの入力情報を取得する
	result = keyboard->GetDeviceState(sizeof(key), key);



	// マウスの入力状態を取得
	result = mouse->Acquire();
	if (SUCCEEDED(result)) {
		mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}

	GetCursorPos(&mousePos);
	ScreenToClient(winApp->GetHWND(), &mousePos);
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::Triggerkey(BYTE keyNumber)
{
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;

	}
	return false;
}