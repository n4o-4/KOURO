#pragma once
#include <memory>
#include <functional>

/**
* \brief イベントクラス
*/

class Event
{
public: // メンバ関数

	/// \brief イベントの実行
	void Execute() { if (callback_) callback_(); }

	/**
	* \brief コールバック関数の設定
	* \param callback コールバック関数
	*/
	void SetCallback(const std::function<void()>& callback) { callback_ = callback; }

private: // メンバ変数

	// イベントのコールバック関数
	std::function<void()> callback_;

};