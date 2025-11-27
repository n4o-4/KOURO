#pragma once

#include "BaseLineObject.h"

// \brief LineObjectManager
// 複数のラインオブジェクトを管理するクラス。
// LineObjectの追加や更新処理を一括で行う。
// 内部でstd::listにラインオブジェクトを保持。

class LineObjectManager
{
public:

	void Update();

	void AddLineObject(std::unique_ptr<BaseLineObject> lineObject);

private:

	std::list<std::unique_ptr<BaseLineObject>> lineObjects_;

};

