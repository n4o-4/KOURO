#include "BaseEffect.h"

void BaseEffect::Initialize()
{
	// パイプラインの生成
	pipeline_ = std::make_unique<Pipeline>();
}
