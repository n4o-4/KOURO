#include "BaseEffect.h"

void BaseEffect::Initialize()
{
	// �p�C�v���C���̐���
	pipeline_ = std::make_unique<Pipeline>();
}
