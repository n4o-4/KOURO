#pragma once

#include "BaseLineObject.h"

/// <summary>
/// LineObjectの管理クラス
/// LineObjectの更新を行う
/// </summary>

class LineObjectManager
{
public:

	void Update();

	void AddLineObject(std::unique_ptr<BaseLineObject> lineObject);

private:

	std::list<std::unique_ptr<BaseLineObject>> lineObjects_;

};

