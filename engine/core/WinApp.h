#pragma once
#include <Windows.h>
#include <cstdint>
#include "imgui.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// \brief WinApp /// Windowsアプリケーションのウィンドウ生成とメッセージ処理を管理するクラス。 HWNDやウィンドウサイズなど、DirectX初期化の基盤となる情報を提供する。

class WinApp
{
public: // 静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	
	/// \brief 初期化
	void Initialize();

	/// \brief 終了処理
	void Finalize();
	
	/**
	* \brief  ウィンドウハンドル取得
	* \return ウィンドウハンドル
	*/
	HWND GetHWND() const { return hwnd; }

	/**
	* \brief  HINSTANCE取得
	* \return HINSTANCE
	*/
	HINSTANCE GetHInstance() const { return wc.hInstance; }

	/**
	* \brief  メッセージ処理
	* \return メッセージが存在していたらtrue、なければfalse
	*/
	bool ProcessMessage();

public: // 定数
	// クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	HWND hwnd = nullptr;

	WNDCLASS wc{};
};

