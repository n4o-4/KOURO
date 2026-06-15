#pragma once

// standard library
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Vector2.h"

namespace Kouro
{
	class WinApp;
}

namespace Kouro
{
	class Mouse
	{
	public:

		/**
		* \brief  MouseButton マウスのボタンを表す列挙型
		* LEFT: 左ボタン、RIGHT: 右ボタン、MIDDLE: 中央ボタン
		*/
		enum class MouseButton
		{
			LEFT = 0,
			RIGHT = 1,
			MIDDLE = 2,
		};

	public:

		/**
		* \brief  Initialize マウスの初期化
		* \param  directInput DirectInputのポインタ
		* \param  winApp WinAppのポインタ
		*/
		void Initialize(IDirectInput8* directInput, WinApp* winApp);

		// \brief  Update マウスの状態更新
		void Update();

		/**
		* \brief  PushMouseButton マウスのボタンが押されているかを判定
		* \param  mouseButton 判定するマウスボタン
		*/
		bool PushButton(MouseButton mouseButton);

		/**
		* \brief  TriggerMouseButton マウスのボタンが押された瞬間を判定
		* \param  mouseButton 判定するマウスボタン
		*/
		bool TriggerButton(MouseButton mouseButton);
		
		/**
		* \brief  GetDelta マウスの移動量を取得
		* \return マウスの移動量
		*/
		Vector2 GetDelta() { return Vector2(static_cast<float>(currentMouseState_.lX), static_cast<float>(currentMouseState_.lY)); }

		/**
		* \brief  GetWheelDelta マウスホイールの回転量を取得
		* \return マウスホイールの回転量
		*/
		float GetWheelDelta() { return static_cast<float>(currentMouseState_.lZ); }

	private:

		Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_; // マウスデバイス
		DIMOUSESTATE2 currentMouseState_ = {}; // 現在のマウスの状態を格納する構造体
		DIMOUSESTATE2 previousMouseState_ = {}; // 前回のマウスの状態を格納する構造体
	};
}