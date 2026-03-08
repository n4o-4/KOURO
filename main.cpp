#include "LeakChecker.h"
#include"MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// リークチェッカーのインスタンスを生成
	Kouro::LeakChecker leakChecker;
	
	// ゲームのインスタンスを生成
	std::unique_ptr<MyGame> myGame = std::make_unique<MyGame>();

	// ゲームの実行
	myGame->Run();

	return 0;
}