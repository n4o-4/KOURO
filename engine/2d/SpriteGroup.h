#pragma once
// standard library
#include <string>
#include <memory>
#include <unordered_map>

// engine
#include "Sprite.h"

namespace Kouro
{
	// / \brief スプライトグループ

	struct SpriteGroup
	{
		std::unordered_map <std::string,std::unique_ptr<Kouro::Sprite>> sprites; //!< スプライトのリスト
		bool isVisible = false;                               //!< グループの表示フラグ
	};
}