#pragma once
#include <type_traits>
#include <vector>
#include <memory>
#include <chrono>
#include "Sprite.h"

template <typename T>
class NumUi
{
public:
    void Initialize()
    {
        value_ = static_cast<T>(0);

        for (size_t i = 0; i < 2; ++i)
        {
            std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
            sprite->Initialize(SpriteCommon::GetInstance(), "Resources/num.png");
			sprite->SetTexSize({ 64.0f,128.0f });
			sprite->SetAnchorPoint({ 0.0f,0.0f });
			numSprites_.push_back(std::move(sprite));
        }
    }

    void Update(T newValue)
    {
        value_ = newValue;

		size_t digitCount = 0;
		digitCount = GetDigitCount(value_);

		size_t temp = value_;
		while (temp > 0)
		{
			int digit = temp % 10;
			digits_.push_back(digit);
			temp /= 10;
		}

		// êîéöÇÃÉäÉXÉgÇãtèáÇ…Ç∑ÇÈ
		std::reverse(digits_.begin(), digits_.end());

		for (size_t i = 0; i < numSprites_.size();)
		{
			if (i < digitCount)
			{
				numSprites_[i]->SetSize({ texWidth_, texHeight_ });

				Vector2 texLeftTop = { 0.0f,digits_[i] * 128.0f };

				numSprites_[i]->SetTexLeftTop(texLeftTop);

				numSprites_[i]->SetPosition({ position_.x + (texWidth_ + digitSpacing_) * static_cast<float>(i), position_.y });

				numSprites_[i]->Update();

				++i;
			}
			else
			{
				//numSprites_.erase(numSprites_.begin() + i);
			}
		}
    }

    void Draw()
    {
		for (auto& sprite : numSprites_)
		{
			sprite->Draw();
		}
    }

	void SetTexSize(float width, float height)
	{
		texWidth_ = width;
		texHeight_ = height;
	}

	void SetDigitSpacing(float spacing)
	{
		digitSpacing_ = spacing;
	}

private:

	size_t GetDigitCount(T value)
	{
		size_t count = 0;
		do
		{
			value /= 10;
			++count;
		} while (value != 0);
		return count;
	}


private:

	Vector2 position_ = { 0.0f, 0.0f };

    T value_;

	uint32_t distributionValue_ = 0;

	std::vector<std::unique_ptr<Sprite>> numSprites_;

    float digitSpacing_ = 0.0f;

	float texWidth_ = 64.0f;

	float texHeight_ = 128.0f;

	std::vector<int> digits_;

	bool isCountUp_ = false;
};