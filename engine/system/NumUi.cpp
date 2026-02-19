#include "NumUi.h"

namespace Kouro
{
    void NumUi::Initialize(size_t maxDigits)
    {
        for (size_t i = 0; i < maxDigits; ++i)
        {
            std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
            sprite->Initialize(Kouro::SpriteCommon::GetInstance(), "texture/num.png");
            sprite->SetTexSize({ 64.0f,128.0f });
            sprite->SetAnchorPoint({ 0.0f,0.0f });
            numSprites_.push_back(std::move(sprite));
        }

        position_ = { 800.0f, 550.0f };
    }

    void NumUi::Update()
    {
        if (!isCount_) return;

        // 経過時間を取得
        float elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - lastUpdateTime_).count();

        // 一定時間経過ごとに値を更新
        if (elapsed >= countDuration_)
        {
            lastUpdateTime_ = std::chrono::steady_clock::now();

            if (destinationValue_ > value_)
            {
                value_++;
            }
            else if (destinationValue_ < value_)
            {
                value_--;
            }

            // 到達したら終了
            if (value_ == destinationValue_)
            {
                isCount_ = false;
                isCountFinished_ = true;
            }
        }

        // 桁分解（毎フレーム行う）
        digits_.clear();

        uint32_t tempValue = value_;

        // 値を1桁ずつ分解
        if (tempValue == 0)
        {
            digits_.push_back(0);
        }
        else
        {
            while (tempValue > 0)
            {
                digits_.push_back(tempValue % 10);
                tempValue /= 10;
            }
        }

        // ---- ここから追加 ----
        std::reverse(digits_.begin(), digits_.end());
        // -----------------------

        // 2桁に揃える（ゼロ埋め）
        while (digits_.size() < 2)
        {
            digits_.insert(digits_.begin(), 0);
        }

        // スプライト更新
        for (size_t i = 0; i < numSprites_.size() && i < digits_.size(); ++i)
        {
            int digit = digits_[i];
            numSprites_[i]->SetTexLeftTop({ 0.0f, static_cast<float>(digit) * texHeight_ });
            numSprites_[i]->SetSize({ texWidth_, texHeight_ });
            numSprites_[i]->SetPosition({ position_.x + i * (texWidth_ + digitSpacing_), position_.y });
            numSprites_[i]->Update();
        }
    }

    void NumUi::Draw()
    {

        // スプライトの位置とテクスチャを更新して描画
        for (size_t i = 0; i < numSprites_.size(); ++i)
        {
            numSprites_[i]->Draw();
        }
    }

    void NumUi::SetDestinationValue(uint32_t value, float countTime)
    {
        destinationValue_ = value;

        if (destinationValue_ != value_)
        {
            isCount_ = true;
            lastUpdateTime_ = std::chrono::steady_clock::now();

            // カウント差
            int diff = static_cast<int>(destinationValue_) - static_cast<int>(value_);

            // ステップ数（絶対値）
            int steps = std::abs(diff);
            if (steps == 0) steps = 1; // 0除算防止

            // 1ステップにかける時間
            countDuration_ = countTime / static_cast<float>(steps);
        }
    }
}