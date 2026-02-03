#pragma once
#include "Sprite.h"

namespace Kouro
{
	class UIController
	{
	public:

		virtual ~UIController() = default;

		virtual void Initialize(Sprite* sprite) = 0;

	protected:

		Sprite* sprite_;

	};
}