#pragma once
#include "Vector3.h"

struct Vector2 {
	float x;
	float y;

	Vector2(float x = 0,float y = 0) : x(x), y(y){}


	Vector2 operator+=(const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
};

struct Vector4 
{
	float x;
	float y;
	float z;
	float w;

   //Vector4(float x = 0, float y = 0, float z = 0,float w = 0) : x(x), y(y), z(z), w(w) {}
   constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
	   : x(x), y(y), z(z), w(w) {
   }
};

inline Vector4 operator+(const Vector4& v1, const Vector4& v2) { return Vector4{ v1.x + v2.x,v1.y + v2.y,v1.z + v2.z,v1.w + v2.w }; }
inline Vector4 operator*(const Vector4& v1, float s) 
{ 
	return Vector4{ 
		v1.x * s,
		v1.y * s,
		v1.z * s,
		v1.w * s }; 
}