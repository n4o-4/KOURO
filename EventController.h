#pragma once
#include <string>
#include <unordered_map>

#include "Event.h"

/**
* \brief イベントコントローラークラス
*/

class EventController
{
public: // メンバ関数

	/**
	* \brief  イベントの追加
	* \param  name イベントの名前
	* \param  event イベントのポインタ
	*/
	void AddEvent(const std::string& name, std::unique_ptr<Event> event) { events_[name] = std::move(event); }

	/**
	* \brief  イベントの実行
	* \param  name : イベントの名前
	*/
	void ExecuteEvent(const std::string& name);

private: // メンバ変数

	std::unordered_map<std::string, std::unique_ptr<Event>> events_; // イベントのマップ

};