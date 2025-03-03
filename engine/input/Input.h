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
#pragma comment(lib, "Xinput9_1_0.lib")
#include <memory>
#include "WinApp.h"
#include "MyMath.h"	


using namespace Microsoft::WRL;

// 入力
class Input
{

public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	enum class MouseButton
	{
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
	};

	enum class GamePadButton
	{
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB,
		RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
		LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
		DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	};

public: // メンバ変数
	// シングルトンインスタンスの取得
	static Input* GetInstance();

	// 初期化
	void Initialize(WinApp* winApp);

	void Finalize();

	// 更新
	void Update();

	/// キーボード
	bool PushKey(BYTE keyNumber);

	bool Triggerkey(BYTE keyNumber);


	/// マウス
	Vector2 GetMousePos() { return Vector2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); }

	bool PushMouseButton(MouseButton mouseButton);

	/// ゲームパッド
	bool PushGamePadButton(GamePadButton button);

	Vector3 GetLeftStick() { return leftStick; }	

private:

	void MouseUpdate();

	void KeyBoardUpdate();

	void GamePadUpdate();	

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

public:

	// マウスデバイス
	ComPtr<IDirectInputDevice8> mouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	POINT mousePos = {};

private:

	// ゲームパッド
	XINPUT_STATE gamePadState = {};

	XINPUT_STATE gamePadStatePre = {};

	Vector3 leftStick = {};

	// WindowAPI
	WinApp* winApp = nullptr;
};