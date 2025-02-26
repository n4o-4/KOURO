#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2(float x = 0,float y = 0) : x(x), y(y){}

	Vector2 operator+=(const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
};

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
};

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3{ v1.x + v2.x,v1.y + v2.y,v1.z + v2.z }; }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3{ v1.x - v2.x,v1.y - v2.y,v1.z - v2.z }; }
inline Vector3 operator*(const Vector3& v1, float s) { return Vector3{ v1.x * s,v1.y * s,v1.z * s }; }

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

   //Vector4(float x = 0, float y = 0, float z = 0,float w = 0) : x(x), y(y), z(z), w(0) {}
};