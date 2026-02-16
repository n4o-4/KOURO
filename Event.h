#pragma once
class Event
{
public:
	/// \brief デストラクター
	virtual ~Event() = default;

	/// \brief イベントの実行
	virtual void Execute() = 0;
};

