#pragma once

// standard library

#include <Windows.h>
#include <XInput.h>

// engine

#include "Vector2.h"

namespace Kouro
{
	class GamePad
	{
	public:

		const float kMaxStickValue_ = 32767.0f; // スティックの最大値

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

	public:

		// \brief Initialize ゲームパッドの初期化
		void Initialize();

		// \brief Update ゲームパッドの状態更新
		void Update();

		/**
		* \brief IsConnected ゲームパッドが接続されているかを判定
		* \return ゲームパッドが接続されている場合はtrue、そうでない場合はfalse
		*/
		bool IsConnected() const { return isConnected_; }

		/**
		* \brief PushButton 引数のボタンが現在押されているかを判定
		* \param button 判定するボタンの種類
		* \return ボタンが押されている場合はtrue、そうでない場合はfalse
		*/
		bool PushButton(GamePadButton button) const;

		/**
		* \brief TriggerButton 引数のボタンが押された瞬間を判定
		* \param button 判定するボタンの種類
		* \return ボタンが押された瞬間の場合はtrue、そうでない場合はfalse
		*/
		bool TriggerButton(GamePadButton button) const;

		/**
		* \brief GetLeftStick 左スティックの入力値を取得
		* \return 左スティックの入力値
		*/
		Vector2 GetLeftStick() const { return leftStick_; }

		/**
		* \brief GetRightStick 右スティックの入力値を取得
		* \return 右スティックの入力値
		*/
		Vector2 GetRightStick() const { return rightStick_; }

		bool PushLeftTrigger() const { return currentState_.Gamepad.bLeftTrigger > 0; }

		bool PushRightTrigger() const { return currentState_.Gamepad.bRightTrigger > 0; }

	private:
		/**
		* \brief スティックの入力値を計算する関数
		* \param x スティックのX軸の値
		* \param y スティックのY軸の値
		* \param deadZone デッドゾーンの値
		* \return デッドゾーンを考慮したスティックの入力値（-1.0f 〜 1.0f）
		*/
		Vector2 CalculateStick(SHORT x, SHORT y, float deadZone);
		
	private:

		bool isConnected_ = false; // ゲームパッドが接続されているかのフラグ

		XINPUT_STATE currentState_ = {}; // 現在のゲームパッドの状態
		XINPUT_STATE previousState_ = {}; // 前回のゲームパッドの状態

		Vector2 leftStick_ = { 0.0f, 0.0f }; // 左スティックの入力値
		Vector2 rightStick_ = { 0.0f, 0.0f }; // 右スティックの入力値

		float deadZone_ = 8000.0f;  // デッドゾーン（反応しない範囲）
	};
}