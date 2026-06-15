#pragma once

// standard library
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Kouro
{
	class WinApp;
}

namespace Kouro
{
	class KeyBoard
	{
	public:

		/**
		* \brief  Initialize キーボードの初期化
		* \param  directInput DirectInputのポインタ
		* \param  winApp WinAppのポインタ
		*/
		void Initialize(IDirectInput8* directInput, WinApp* winApp);

		// \brief  Update キーボードの状態更新
		void Update();

		/**
		* \brief  PushKey 引数のキーが現在押されているかを判定
		* \param  keyNumber 判定するキーの番号
		* \return キーが押されている場合はtrue、そうでない場合はfalse
		*/
		bool PushKey(BYTE keyNumber);
		
		/**
		* \brief 引数のキーが押された瞬間を判定
		* \param keyNumber 判定するキーの番号
		* \return キーが押された瞬間の場合はtrue、そうでない場合はfalse
		*/
		bool TriggerKey(BYTE keyNumber);

	private:

		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_; // キーボードデバイス

		BYTE currentKeys_[256] = {}; // 現在のキーの状態を格納する配列
		BYTE previousKeys_[256] = {}; // 前回のキーの状態を格納する配列
	};
}