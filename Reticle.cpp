#include "Reticle.h"

void Reticle::Initialize()
{
	// ワールドトランスフォームの初期化
	worldTransform_ = std::make_unique<Kouro::WorldTransform>();


}

void Reticle::Update(Kouro::Input input)
{
}
