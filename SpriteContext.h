#pragma once
#include <unordered_map>
#include <functional>

#include "Sprite.h"

namespace Kouro
{
	namespace SpriteContext
	{
		struct VertexData
		{
			Vector4 position;
			Vector2 texcoord;
			Vector3 normal;
		};

		/**
		* \brief  スプライトエントリー
		* \param  sprite     : スプライトのポインタ
		* \param  updateFunc : スプライトの更新関数（引数にスプライトを受け取る関数オブジェクト）
		*/
		struct SpriteEntry
		{
			std::unique_ptr<Kouro::Sprite> sprite;          //!< スプライトのポインタ
			std::function<void(Kouro::Sprite&)> updateFunc; //!< スプライトの更新関数
		};

		/**
		* \brief  スプライトグループ
		* \param  sprites : スプライトのリスト（名前をキーとしたマップ）
		* \param  isVisible : グループの表示フラグ
		*/
		struct SpriteGroup
		{
			std::unordered_map < std::string, SpriteEntry> sprites; //!< スプライトのリスト
			bool isVisible = false;                                 //!< グループの表示フラグ
		};

		/**
		* \brief  グループ名をキーとしたスプライトグループのマップ
		* \param  string      : グループ名
		* \param  SpriteGroup : スプライトのリストと表示フラグを持つ構造体
		*/
		using SpriteGroups = std::unordered_map<std::string, SpriteContext::SpriteGroup>;
	}
}