#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

	//Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
	constexpr Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: x(x), y(y), z(z) {
	}

	// ?? **ベクトルの加算代入演算子を追加**
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	// ?? **ベクトルの減算代入演算子を追加**
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	// ?? スカラー乗算代入演算子を追加
	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	// ?? **単項マイナス演算子を追加**
	//Vector3 operator-() const { return Vector3(-x, -y, -z); }

	Vector3 operator/(float scalar) const {
		return { x / scalar, y / scalar, z / scalar };
	}
};

inline Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z }; }

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z }; }

inline Vector3 operator*(float s, const Vector3& v) { return { s * v.x, s * v.y, s * v.z }; }

inline Vector3 operator*(const Vector3& v, float s) { return s * v; }