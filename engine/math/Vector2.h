#pragma once

namespace Kouro
{
	struct Vector2 {
		float x;
		float y;

		Vector2(float x = 0, float y = 0) : x(x), y(y) {}

		Vector2 operator+=(const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
	};
}