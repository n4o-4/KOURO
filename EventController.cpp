#include "EventController.h"

void EventController::ExecuteEvent(const std::string& name)
{
	// イベントの名前に対応するイベントを検索
	auto it = events_.find(name);

	// イベントが見つかった場合は、イベントを実行
	if (it != events_.end())
	{
		// イベントを実行
		it->second->Execute();
	}
}