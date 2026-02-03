#pragma once
#include <functional>

#include "MyMath.h"
#include "Sprite.h"
#include "Player.h"

/// \brief Kouro::SpriteMotionLibrary スプライトモーションライブラリ名前空間
/// 汎用関数のみ
namespace Kouro::SpriteMotionLibrary
{
    // 汎用色補間クロージャ
    inline std::function<void(Kouro::Sprite&, float)> LerpColor(const Kouro::Vector4& startColor,const Kouro::Vector4& endColor)
    {
        return [startColor, endColor](Kouro::Sprite& sprite, float t)
            {
                Kouro::Vector4 currentColor = Kouro::Lerp(startColor, endColor, t);
                sprite.SetColor(currentColor);
            };
    }
}

/// \brief PlayerUI プレイヤーUI名前空間
/// ゲーム固有
namespace PlayerUI
{
    inline std::function<void(Kouro::Sprite&)> FireUI(const Kouro::Vector4& startColor, const Kouro::Vector4& endColor,Player* player)
    {
        return [startColor, endColor, player](Kouro::Sprite& sprite)
            {
                if (!player) return;

                float t = std::clamp(player->GetFireIntervalTimer() / player->GetFireInterval(), 0.0f, 1.0f);

				Kouro::SpriteMotionLibrary::LerpColor(startColor, endColor)(sprite, t);
            };
	}


}