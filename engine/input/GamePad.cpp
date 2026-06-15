#include "GamePad.h"

// standard library

#include <algorithm>

// engine
#include "MyMath.h"

void Kouro::GamePad::Initialize()
{
	// ゲームパッドの接続状態を確認
	XINPUT_STATE state;
	DWORD result = XInputGetState(0, &state);
	isConnected_ = (result == ERROR_SUCCESS);
}

void Kouro::GamePad::Update()
{
	previousState_ = currentState_;

	DWORD result = XInputGetState(0, &currentState_);

	if (result != ERROR_SUCCESS)
	{
		ZeroMemory(&currentState_, sizeof(currentState_));
		isConnected_ = false;
		return;
	}

	isConnected_ = true;

	// スティック計算
	leftStick_ = CalculateStick(currentState_.Gamepad.sThumbLX, currentState_.Gamepad.sThumbLY, deadZone_);
	rightStick_ = CalculateStick(currentState_.Gamepad.sThumbRX, currentState_.Gamepad.sThumbRY, deadZone_);
}

bool Kouro::GamePad::PushButton(GamePadButton button) const
{
	return (currentState_.Gamepad.wButtons & static_cast<WORD>(button)) != 0;
}

bool Kouro::GamePad::TriggerButton(GamePadButton button) const
{
	return PushButton(button) && !(previousState_.Gamepad.wButtons & static_cast<WORD>(button));
}

Kouro::Vector2 Kouro::GamePad::CalculateStick(SHORT x, SHORT y, float deadZone)
{
	Vector2 stick =
	{
		static_cast<float>(x),
		static_cast<float>(y)
	};

	float length = Length(stick);

	if (length < deadZone)
	{
		return { 0.0f, 0.0f };
	}

	return stick / kMaxStickValue_;
}