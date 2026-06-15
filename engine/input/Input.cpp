#include "Input.h"
#include "KeyBoard.h"

namespace Kouro
{
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
		result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
		assert(SUCCEEDED(result));

		// キーボードデバイスの作成と初期化
		keyBoard_ = std::make_unique<KeyBoard>();
		keyBoard_->Initialize(directInput_.Get(), winApp);

		// マウスデバイスの作成と初期化
		mouse_ = std::make_unique<Mouse>();
		mouse_->Initialize(directInput_.Get(), winApp);
	
		// ゲームパッドデバイスの作成と初期化
		gamePad_ = std::make_unique<GamePad>();
		gamePad_->Initialize();
	}

	void Input::Update()
	{
		if (!isReception_)
		{
			return;
		}

		keyBoard_->Update();
		mouse_->Update();
		gamePad_->Update();
	}
}