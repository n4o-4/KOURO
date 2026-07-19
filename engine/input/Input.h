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

#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"

using namespace Microsoft::WRL;

// \brief Input  
// キーボード・マウス・ゲームパッドなど、あらゆる入力デバイスの状態を統合的に管理するクラス。  
// 押下・トリガー判定やスティック入力の取得を行い、ゲーム操作全般をサポートする。

namespace Kouro
{
	class Input
	{
	public:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	public:


	public: // メンバ変数

		/// \brief GetInstance インスタンスの取得
		static Input* GetInstance();

		/**
		* \brief  Initialize 初期化
		* \param  winApp WinAppのポインタ
		*/
		void Initialize(WinApp* winApp);

		/// \brief  Update 入力情報の更新
		void Update();

		/**
		* \brief  SetIsReception 入力受付フラグの設定
		* \param  flag 入力受付フラグ
		*/
		void SetIsReception(bool flag) { isReception_ = flag; }

		/// キーボード
		/**
		* \brief  PushKey 引数のキーが現在押されているかを判定
		* \param  keyNumber 判定するキーの番号
		* \return キーが押されている場合はtrue、そうでない場合はfalse
		*/
		bool PushKey(BYTE keyNumber) { return keyBoard_->PushKey(keyNumber); }
		
		/**
		* \brief  TriggerKey 引数のキーが押された瞬間かを判定
		* \param  keyNumber 判定するキーの番号
		* \return キーが押された瞬間の場合はtrue、そうでない場合はfalse
		*/
		bool TriggerKey(BYTE keyNumber) { return keyBoard_->TriggerKey(keyNumber); }		

		/// マウス
		/**
		* \brief  PushButton 引数のマウスボタンが現在押されているかを判定
		* \param  mouseButton 判定するマウスボタン
		* \return マウスボタンが押されている場合はtrue、そうでない場合はfalse
		*/
		bool PushButton(Mouse::MouseButton mouseButton) { return mouse_->PushButton(mouseButton); }

		/**
		* \brief  TriggerButton 引数のマウスボタンが押された瞬間かを判定
		* \param  mouseButton 判定するマウスボタン
		* \return マウスボタンが押された瞬間の場合はtrue、そうでない場合はfalse
		*/
		bool TriggerButton(Mouse::MouseButton mouseButton) { return mouse_->TriggerButton(mouseButton); }

		/**
		* \brief  GetMouseDelta マウスの移動量を取得
		* \return マウスの移動量
		*/
		Vector2 GetMouseDelta() { return mouse_->GetDelta(); }

		/**
		* \brief  GetMouseWheelDelta マウスホイールの回転量を取得
		* \return マウスホイールの回転量
		*/
		float GetMouseWheelDelta() { return mouse_->GetWheelDelta(); }

		/// ゲームパッド
		/**
		* \brief  PushButton 引数のゲームパッドボタンが現在押されているかを判定
		* \param  button 判定するゲームパッドボタン
		* \return ゲームパッドボタンが押されている場合はtrue、そうでない場合はfalse
		*/
		bool PushButton(GamePad::GamePadButton button) { return gamePad_->PushButton(button); }

		/**
		* \brief  TriggerButton 引数のゲームパッドボタンが押された瞬間かを判定
		* \param  button 判定するゲームパッドボタン
		* \return ゲームパッドボタンが押された瞬間の場合はtrue、そうでない場合はfalse
		*/
		bool TriggerButton(GamePad::GamePadButton button) { return gamePad_->TriggerButton(button); }

		/**
		* \brief  GetLeftStick 左スティックの入力値を取得
		* \return 左スティックの入力値
		*/
		Vector2 GetLeftStick() { return gamePad_->GetLeftStick(); }

		/**
		* \brief  GetRightStick 右スティックの入力値を取得
		* \return 右スティックの入力値
		*/
		Vector2 GetRightStick() { return gamePad_->GetRightStick(); }

		bool GetLeftTrigger() { return gamePad_->PushLeftTrigger(); }
		bool GetRightTrigger() { return gamePad_->PushRightTrigger(); }

	private:

		static std::unique_ptr<Input> instance;

		friend std::unique_ptr<Input> std::make_unique<Input>();
		friend std::default_delete<Input>;

		Input() = default;
		~Input() = default;
		Input(Input&) = delete;
		Input& operator=(Input&) = delete;

		//　入力受付フラグ
		bool isReception_ = true;

		// WindowAPI
		WinApp* winApp = nullptr;

		ComPtr<IDirectInput8> directInput_;

		std::unique_ptr<KeyBoard> keyBoard_ = nullptr; // キーボードデバイス
		std::unique_ptr<Mouse> mouse_ = nullptr; // マウスデバイス
		std::unique_ptr<GamePad> gamePad_ = nullptr; // ゲームパッドデバイス

	};
}