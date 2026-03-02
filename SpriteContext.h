#pragma once
#include <unordered_map>

#include "Sprite.h"

namespace Kouro
{
	namespace SpriteContext
	{
		struct SpriteGroup
		{
			std::unordered_map < std::string, std::tuple<std::unique_ptr<Kouro::Sprite>, std::function<void(Kouro::Sprite&)>>> sprites; //!< スプライトのリスト
			bool isVisible = false;                               //!< グループの表示フラグ
		};
	}
}