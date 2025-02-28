#pragma once
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include<cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <memory>
#include "WinApp.h"

#include "Vectors.h"

using namespace Microsoft::WRL;

// 入力
class Input
{

public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ変数
	// シングルトンインスタンスの取得
	static Input* GetInstance();

	// 初期化
	void Initialize(WinApp* winApp);

	void Finalize();

	// 更新
	void Update();

	bool PushKey(BYTE keyNumber);

	bool Triggerkey(BYTE keyNumber);

	Vector2 GetMousePos() { return Vector2(mousePos.x, mousePos.y); }

private:

	static std::unique_ptr<Input> instance;

	friend std::unique_ptr<Input> std::make_unique<Input>();
	friend std::default_delete<Input>;

	Input() = default;
	~Input() = default;
	Input(Input&) = delete;
	Input& operator=(Input&) = delete;

	// キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	ComPtr<IDirectInput8> directInput;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};

	// マウスデバイス
	ComPtr<IDirectInputDevice8> mouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	POINT mousePos = {};

	// WindowAPI
	WinApp* winApp = nullptr;
};