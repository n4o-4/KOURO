#pragma once
#include "Structs.h"
#include <cmath>
#include <algorithm>
#include <numbers>
#include <cassert>
#include <vector>


const float kDeltaTime = 1.0f / 60.0f;

struct AABB {
	Vector3 center; //!< 中心座標
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

struct Sphere {
	Vector3 center; //!< 中心
	float radius =1.0f; //!< 半径
};

struct OBB
{
	Vector3 center; //!< 中心座標
	Vector3 halfSize; //!< 半分のサイズ（半径）
	Vector3 axes[3]; //!< OBBの軸（X, Y, Z方向の単位ベクトル）
};

inline Vector3 Normalize(Vector3 v)
{
	Vector3 result;

	// ベクトルの長さを計算
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	// 長さが 0 の場合、NaN を防ぐためにそのまま返す
	if (length == 0.0f)
	{
		return v; // ゼロベクトルはそのまま返す
	}

	// 長さが 0 でない場合、正規化
	result = { v.x / length, v.y / length, v.z / length };

	return result;
}

inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

inline float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float LengthSq(const Vector3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline Vector3 Perpendicular(const Vector3& v) {
	// 垂直なベクトルを生成するための基準ベクトル
	Vector3 reference = (std::fabs(v.x) > std::fabs(v.z)) ? Vector3(0, 0, 1) : Vector3(1, 0, 0);

	// 外積を計算して垂直なベクトルを取得
	return Cross(v, reference);
}

inline Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return {
		t * v1.x + (1.0f - t) * v2.x,
		t * v1.y + (1.0f - t) * v2.y,
		t * v1.z + (1.0f - t) * v2.z };
}

inline float fLerp(float startPos, float endPos, float t) {
	return { startPos * (1.0f - t) + t * endPos };
}

inline float LerpShortAngle(float a, float b, float t) {
	float diff = b - a;

	diff = std::fmod(diff, static_cast<float>(std::numbers::pi) * 2.0f);

	if (diff > static_cast<float>(std::numbers::pi)) {
		diff += (static_cast<float>(std::numbers::pi) * -2.0f);
	}

	if (diff < static_cast<float>(-std::numbers::pi)) {
		diff += (static_cast<float>(std::numbers::pi) * 2.0f);
	}

	return fLerp(a, a + diff, t);
}

static Vector3 TransformNormal(const Vector3& normal, const Matrix4x4& mat) {
	Vector3 result;
	result.x = normal.x * mat.m[0][0] + normal.y * mat.m[1][0] + normal.z * mat.m[2][0];
	result.y = normal.x * mat.m[0][1] + normal.y * mat.m[1][1] + normal.z * mat.m[2][1];
	result.z = normal.x * mat.m[0][2] + normal.y * mat.m[1][2] + normal.z * mat.m[2][2];
	return result;
}

static Matrix4x4 MakeRotateXMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = 1;
	rM.m[1][1] = cosf(rotate);
	rM.m[1][2] = sinf(rotate);
	rM.m[2][1] = -sinf(rotate);
	rM.m[2][2] = cosf(rotate);
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 MakeRotateYMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = cosf(rotate);
	rM.m[0][2] = -sinf(rotate);
	rM.m[1][1] = 1;
	rM.m[2][0] = sinf(rotate);
	rM.m[2][2] = cosf(rotate);
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 MakeRotateZMatrix(float rotate)
{
	Matrix4x4 rM{};
	rM.m[0][0] = cosf(rotate);
	rM.m[0][1] = sinf(rotate);
	rM.m[1][0] = -sinf(rotate);
	rM.m[1][1] = cosf(rotate);
	rM.m[2][2] = 1;
	rM.m[3][3] = 1;

	return rM;
}

static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2)
{
	Matrix4x4 resultMatrix{};

	// 行列の掛け算
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float num = 0.0f; // 内側のループで num を初期化する
			for (int k = 0; k < 4; k++)
			{
				num += m1.m[i][k] * m2.m[k][j];
			}
			resultMatrix.m[i][j] = num;
		}
	}

	return resultMatrix;
}

static Matrix4x4 MakeRotateMatrix(const Vector3& rotate)
{
	Matrix4x4 rXM = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rYM = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rZM = MakeRotateZMatrix(rotate.z);

	return Multiply(rXM, Multiply(rYM, rZM));
}

static Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix;

	scaleMatrix.m[0][0] = scale.x;  // x軸のスケール
	scaleMatrix.m[0][1] = 0.0f;     // x軸とy軸のオフセット
	scaleMatrix.m[0][2] = 0.0f;     // x軸とz軸のオフセット
	scaleMatrix.m[0][3] = 0.0f;     // 位置情報

	scaleMatrix.m[1][0] = 0.0f;     // y軸とx軸のオフセット
	scaleMatrix.m[1][1] = scale.y;  // y軸のスケール
	scaleMatrix.m[1][2] = 0.0f;     // y軸とz軸のオフセット
	scaleMatrix.m[1][3] = 0.0f;     // 位置情報

	scaleMatrix.m[2][0] = 0.0f;     // z軸とx軸のオフセット
	scaleMatrix.m[2][1] = 0.0f;     // z軸とy軸のオフセット
	scaleMatrix.m[2][2] = scale.z;  // z軸のスケール
	scaleMatrix.m[2][3] = 0.0f;     // 位置情報

	scaleMatrix.m[3][0] = 0.0f;     // 位置情報
	scaleMatrix.m[3][1] = 0.0f;     // 位置情報
	scaleMatrix.m[3][2] = 0.0f;     // 位置情報
	scaleMatrix.m[3][3] = 1.0f;     // 同次座標の一部

	return scaleMatrix;
}

static Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	// 平行移動行列の作成
	Matrix4x4 translateMatrix = {};

	// 対角成分を 1.0 にする（単位行列の作成）
	translateMatrix.m[0][0] = 1.0f;
	translateMatrix.m[1][1] = 1.0f;
	translateMatrix.m[2][2] = 1.0f;
	translateMatrix.m[3][3] = 1.0f;

	// 平行移動成分をセット
	translateMatrix.m[3][0] = translate.x;  // X方向の移動
	translateMatrix.m[3][1] = translate.y;  // Y方向の移動
	translateMatrix.m[3][2] = translate.z;  // Z方向の移動
	return translateMatrix;
}

static Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 Translate)
{
	Matrix4x4 sM{};
	Matrix4x4 tM{};
	Matrix4x4 resultMatrix{};

	//scaleMatrix
	sM.m[0][0] = scale.x;
	sM.m[0][1] = 0;
	sM.m[0][2] = 0;
	sM.m[1][0] = 0;
	sM.m[1][1] = scale.y;
	sM.m[1][2] = 0;
	sM.m[2][0] = 0;
	sM.m[2][1] = 0;
	sM.m[2][2] = scale.z;
	sM.m[3][3] = 1;

	//rotateMatrix
	Matrix4x4 rXM = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rYM = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rZM = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rXYZM =Multiply(rXM, Multiply(rYM, rZM));
	
	//translateMatrix
	tM.m[0][0] = 1;
	tM.m[1][1] = 1;
	tM.m[2][2] = 1;
	tM.m[3][0] = Translate.x;
	tM.m[3][1] = Translate.y;
	tM.m[3][2] = Translate.z;
	tM.m[3][3] = 1;

	resultMatrix.m[0][0] = sM.m[0][0] * rXYZM.m[0][0];
	resultMatrix.m[0][1] = sM.m[0][0] * rXYZM.m[0][1];
	resultMatrix.m[0][2] = sM.m[0][0] * rXYZM.m[0][2];

	resultMatrix.m[1][0] = sM.m[1][1] * rXYZM.m[1][0];
	resultMatrix.m[1][1] = sM.m[1][1] * rXYZM.m[1][1];
	resultMatrix.m[1][2] = sM.m[1][1] * rXYZM.m[1][2];

	resultMatrix.m[2][0] = sM.m[2][2] * rXYZM.m[2][0];
	resultMatrix.m[2][1] = sM.m[2][2] * rXYZM.m[2][1];
	resultMatrix.m[2][2] = sM.m[2][2] * rXYZM.m[2][2];


	resultMatrix.m[3][0] = tM.m[3][0];
	resultMatrix.m[3][1] = tM.m[3][1];
	resultMatrix.m[3][2] = tM.m[3][2];
	resultMatrix.m[3][3] = 1;

	return resultMatrix;
}

static Matrix4x4 MakeIdentity4x4()
{
	Matrix4x4 resultMatrix{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				resultMatrix.m[i][j] = 1;
			}
			else
			{
				resultMatrix.m[i][j] = 0;
			}
		}
	}
	return resultMatrix;
}

static Matrix4x4 Inverse(Matrix4x4 matrix)
{
	Matrix4x4 resultMatrix{};
	float A =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]

		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]//

		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]

		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]

		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];


	resultMatrix.m[0][0] =
		(matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][1] =
		(-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] +
			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][2] =
		(matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) / A;

	resultMatrix.m[0][3] =
		(-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] +
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) / A;

	resultMatrix.m[1][0] =
		(-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
			matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[1][1] =
		(matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
			matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) / A;

	resultMatrix.m[1][2] =

		(-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] +
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) / A;

	/*resultMatrix.m[1][3] =
		(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) / A;*/

	resultMatrix.m[1][3] =
		(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
			matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] - // 修正: m[3][3] → m[2][3]
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) / A;

	resultMatrix.m[2][0] =
		(matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
			matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
			matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
			matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][1] =
		(-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
			matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
			matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] +
			matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][2] =
		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) / A;

	resultMatrix.m[2][3] =
		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
			matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
			matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] +
			matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) / A;

	resultMatrix.m[3][0] =
		(-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
			matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
			matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
			matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
			matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][1] =
		(matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
			matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
			matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
			matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][2] =
		(-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] +
			matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) / A;

	resultMatrix.m[3][3] =
		(matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
			matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
			matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) / A;

	return resultMatrix;
}

static Matrix4x4 Transpose(const Matrix4x4& mat) {
	Matrix4x4 result{};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] = mat.m[col][row];
		}
	}

	return result;
}

static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRation, float nearClip, float farClip)
{
	float tanHalfFovY = tanf(fovY * 0.5f);
	float scaleX = 1.0f / (aspectRation * tanHalfFovY);
	float scaleY = 1.0f / tanHalfFovY;
	Matrix4x4 resultMatrix{};
	resultMatrix.m[0][0] = scaleX;
	resultMatrix.m[1][1] = scaleY;
	resultMatrix.m[2][2] = farClip / (farClip - nearClip);
	resultMatrix.m[2][3] = 1;
	resultMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return resultMatrix;
}

static Matrix4x4 MakeOrthographicMatrix(float l, float t, float r, float b, float zn, float zf)
{
	Matrix4x4 resultMatrix = {};
	resultMatrix.m[0][0] = 2 / (r - l);
	resultMatrix.m[1][1] = 2 / (t - b);
	resultMatrix.m[2][2] = -2 / (zf - zn);
	resultMatrix.m[3][0] = -(r + l) / (r - l);
	resultMatrix.m[3][1] = -(t + b) / (t - b);
	resultMatrix.m[3][2] = -(zf + zn) / (zf - zn);
	resultMatrix.m[3][3] = 1;

	return resultMatrix;
}

inline bool IsCollision(const AABB& aabb, const Vector3& point)
{
	if ((aabb.min.x <= point.x && aabb.max.x >= point.x) &&
		(aabb.min.y <= point.y && aabb.max.y >= point.y) &&
		(aabb.min.z <= point.z && aabb.max.z >= point.z)) {

		return true;
	}

	return false;
}

static Matrix4x4 MakeRotateAxisAngle(Vector3 axis, float angle)
{
	// 必要な値を事前計算
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);
	float oneMinusCos = 1.0f - cosTheta;

	// 回転行列の各要素を計算
	Matrix4x4 rotationMatrix = {};

	rotationMatrix.m[0][0] = cosTheta + axis.x * axis.x * oneMinusCos;
	rotationMatrix.m[0][1] = axis.y * axis.x * oneMinusCos + axis.z * sinTheta;
	rotationMatrix.m[0][2] = axis.z * axis.x * oneMinusCos - axis.y * sinTheta;
	rotationMatrix.m[0][3] = 0.0f;

	rotationMatrix.m[1][0] = axis.x * axis.y * oneMinusCos - axis.z * sinTheta;
	rotationMatrix.m[1][1] = cosTheta + axis.y * axis.y * oneMinusCos;
	rotationMatrix.m[1][2] = axis.z * axis.y * oneMinusCos + axis.x * sinTheta;
	rotationMatrix.m[1][3] = 0.0f;

	rotationMatrix.m[2][0] = axis.x * axis.z * oneMinusCos + axis.y * sinTheta;
	rotationMatrix.m[2][1] = axis.y * axis.z * oneMinusCos - axis.x * sinTheta;
	rotationMatrix.m[2][2] = cosTheta + axis.z * axis.z * oneMinusCos;
	rotationMatrix.m[2][3] = 0.0f;

	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;


	return rotationMatrix;
}

static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Vector3 fromNormalized = Normalize(from);
	Vector3 toNormalized = Normalize(to);

	// ベクトル間の軸を計算
	Vector3 axis = Cross(fromNormalized, toNormalized);
	float axisLength = Length(axis);

	// 方向が逆の場合
	if (axisLength == 0.0f) {
		if (Dot(fromNormalized, toNormalized) < 0.0f) {
			// 180度回転の場合、適当な垂直軸を選ぶ
			axis = Normalize(Perpendicular(fromNormalized));
		}
		else {
			// 同じ方向の場合、単位行列を返す
			return MakeIdentity4x4();
		}
	}

	axis = Normalize(axis);

	// ベクトル間の角度を計算
	float angle = acosf(std::clamp(Dot(fromNormalized, toNormalized), -1.0f, 1.0f));

	// 回転行列を生成
	return MakeRotateAxisAngle(axis, angle);
}

static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	return Quaternion(
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,  
		lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,  
		lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,  
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z   
	);
}

static Quaternion IdentityQuaternion()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

static Quaternion Conjugate(const Quaternion& quaternion)
{
	// クォータニオンの共役を計算
	return Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
}

// Quaternionのnormを返す
static float Norm(const Quaternion& quaternion)
{
	// クォータニオンのノルムを計算: √(x^2 + y^2 + z^2 + w^2)
	return sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

static Quaternion qNormalize(const Quaternion& quaternion)
{
	// クォータニオンのノルムを計算
	float norm = Norm(quaternion);

	// ノルムが0でないことを確認（ゼロ除算を避ける）
	if (norm > 0.0f)
	{
		// クォータニオンを正規化
		float invNorm = 1.0f / norm;
		return Quaternion(quaternion.x * invNorm, quaternion.y * invNorm, quaternion.z * invNorm, quaternion.w * invNorm);
	}

	// ノルムがゼロの場合、ゼロのクォータニオンを返す
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

static Quaternion Inverse(const Quaternion& quaternion)
{
	// クォータニオンのノルムを計算
	float normSquared = Norm(quaternion) * Norm(quaternion);

	// ノルムの二乗が0でないことを確認（ゼロ除算を避ける）
	if (normSquared > 0.0f)
	{
		// クォータニオンの共役を計算
		Quaternion conjugate = Conjugate(quaternion);

		// 共役をノルムの二乗で割る
		float invNormSquared = 1.0f / normSquared;
		return Quaternion(conjugate.x * invNormSquared, conjugate.y * invNormSquared, conjugate.z * invNormSquared, conjugate.w * invNormSquared);
	}

	// ノルムの二乗が0の場合、ゼロのクォータニオンを返す
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

static float qDot(const Quaternion& q0, const Quaternion& q1)
{
	return q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
}

// 任意回転を表すQuaternionの生成
static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = std::sin(halfAngle);

	return Quaternion(
		axis.x * sinHalfAngle,  // x component
		axis.y * sinHalfAngle,  // y component
		axis.z * sinHalfAngle,  // z component
		std::cos(halfAngle)     // w component
	);
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion q = quaternion;

	Quaternion vectorQuat(vector.x, vector.y, vector.z, 0.0f);
	Quaternion conjugateQuat = Conjugate(q);

	Quaternion rotatedQuat = Multiply(Multiply(quaternion, vectorQuat), conjugateQuat);

	return Vector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);
}

static Matrix4x4 MakeRotateMatrix(const Quaternion& q)
{
	Matrix4x4 matrix;

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	matrix.m[0][0] = ww + xx - yy - zz;
	matrix.m[0][1] = 2.0f * (xy + wz);
	matrix.m[0][2] = 2.0f * (xz - wy);
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 2.0f * (xy - wz);
	matrix.m[1][1] = ww - xx + yy - zz;
	matrix.m[1][2] = 2.0f * (yz + wx);
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 2.0f * (xz + wy);
	matrix.m[2][1] = 2.0f * (yz - wx);
	matrix.m[2][2] = ww - xx - yy + zz;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

static Quaternion qLerp(Quaternion q0, Quaternion q1, float t)
{
	//// クォータニオン間のドット積を計算
	//float dot = quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y + quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;

	// //補間のために、dotが負の値であれば、quaternion2の符号を反転して補間を行う
	//if (dot < 0.0f)
	//{
	//	// クォータニオンの符号を反転
	//	dot = -dot;
	//	quaternion1 = Quaternion(-quaternion2.x, -quaternion2.y, -quaternion2.z, -quaternion2.w);
	//}

	//// 線形補間を計算
	//Quaternion result;
	//if (dot > 0.9995f)
	//{
	//	// ドット積が1に近い場合、単純な補間で十分
	//	result.x = quaternion1.x + t * (quaternion2.x - quaternion1.x);
	//	result.y = quaternion1.y + t * (quaternion2.y - quaternion1.y);
	//	result.z = quaternion1.z + t * (quaternion2.z - quaternion1.z);
	//	result.w = quaternion1.w + t * (quaternion2.w - quaternion1.w);
	//}
	//else
	//{
	//	// クォータニオン間の補間を計算
	//	float theta_0 = acosf(dot);         // 開始と終了の角度
	//	float theta = theta_0 * t;          // 補間割合に基づく角度
	//	float sin_theta = sinf(theta);      // sin(θ)
	//	float sin_theta_0 = sinf(theta_0);  // sin(θ0)

	//	float s1 = cosf(theta) - dot * sin_theta / sin_theta_0;
	//	float s2 = sin_theta / sin_theta_0;

	//	result.x = quaternion1.x * s1 + quaternion2.x * s2;
	//	result.y = quaternion1.y * s1 + quaternion2.y * s2;
	//	result.z = quaternion1.z * s1 + quaternion2.z * s2;
	//	result.w = quaternion1.w * s1 + quaternion2.w * s2;
	//}

	//// 補間結果を正規化して返す
	////result = qNormalize(result);
	//return result;

	Quaternion qu0 = q0;

	Quaternion qu1 = q1;

	 // 内積を求める
	float dot = qDot(qNormalize(qu0),qNormalize(qu1));

	// 内積が負ならq1を反転させて最短経路を取る
	Quaternion adjustedQ1 = q1;
	if (dot < 0.0f) {
		adjustedQ1.x = -q1.x;
		adjustedQ1.y = -q1.y;
		adjustedQ1.z = -q1.z;
		adjustedQ1.w = -q1.w;
		dot = -dot;
	}

	// なす角θを求める
	float theta = std::acos(dot);

	// sin(θ)が0に近い場合はLerpで補間
	if (std::abs(theta) < 1e-6) {
		return Quaternion{
			q0.x * (1.0f - t) + adjustedQ1.x * t,
			q0.y * (1.0f - t) + adjustedQ1.y * t,
			q0.z * (1.0f - t) + adjustedQ1.z * t,
			q0.w * (1.0f - t) + adjustedQ1.w * t
		};
	}

	// Slerpの補間係数を計算
	float sinTheta = std::sin(theta);
	float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	float scale1 = std::sin(t * theta) / sinTheta;

	// 補間結果を求める
	Quaternion result;
	result.x = scale0 * q0.x + scale1 * adjustedQ1.x;
	result.y = scale0 * q0.y + scale1 * adjustedQ1.y;
	result.z = scale0 * q0.z + scale1 * adjustedQ1.z;
	result.w = scale0 * q0.w + scale1 * adjustedQ1.w;

	return result;
}

static Matrix4x4 MakeAffineMatrixforQuater(const Vector3& scale, const Quaternion rotate, const Vector3& translate)
{
	Matrix4x4 result;

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = scaleMatrix * rotateMatrix * translateMatrix;

	return result;
}

static Vector3 qCross(const Quaternion& q1, const Quaternion& q2)
{
	return Vector3(
		q1.y * q2.z - q1.z * q2.y,
		q1.z * q2.x - q1.x * q2.z,
		q1.x * q2.y - q1.y * q2.x
	);
}

static Quaternion EulerToQuaternion(const Vector3& euler) 
{
	float cx = std::cos(euler.x * 0.5f);
	float sx = std::sin(euler.x * 0.5f);
	float cy = std::cos(euler.y * 0.5f);
	float sy = std::sin(euler.y * 0.5f);
	float cz = std::cos(euler.z * 0.5f);
	float sz = std::sin(euler.z * 0.5f);

	Quaternion q;
	q.w = cx * cy * cz + sx * sy * sz;
	q.x = sx * cy * cz - cx * sy * sz;
	q.y = cx * sy * cz + sx * cy * sz;
	q.z = cx * cy * sz - sx * sy * cz;

	return qNormalize(q); // 念のため正規化
}

static Vector3  QuaternionToEuler(const Quaternion& q) 
{
	Vector3 euler;

	// X (Pitch)
	float sinp = 2.0f * (q.w * q.x - q.y * q.z);
	float cosp = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	euler.x = std::atan2(sinp, cosp);

	// Y (Yaw)
	float siny = 2.0f * (q.w * q.y + q.x * q.z);
	float cosy = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	euler.y = std::atan2(siny, cosy);

	// Z (Roll)
	float sinr = 2.0f * (q.w * q.z - q.x * q.y);
	float cosr = 1.0f - 2.0f * (q.z * q.z + q.x * q.x);
	euler.z = std::atan2(sinr, cosr);

	return euler;
}

namespace Vect4
{
	static Vector4 Lerp(const Vector4& a, const Vector4& b, float t)
	{
		return a * (1.0f - t) + b * t;
	}
}


///==============================================================
// rail
///==============================================================

// Catmull-Rom補間を使用して、4つの制御点(p0, p1, p2, p3)を通る曲線上の点を計算する関数
static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	//const float s = 0.5f; // Catmull-Romの係数	

	//float t2 = t * t; // tの2乗

	//float t3 = t2 * t; // tの3乗

	//Vector3 e3 = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
	//Vector3 e2 = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
	//Vector3 e1 = -p0 + p2;
	//Vector3 e0 = 2.0f * p1;

	//return s * (e3 * t3 + e2 * t2 + e1 * t + e0);

	const float alpha = 0.5f;

	// 各点のパラメータ t_i を計算
	auto tj = [&](float tPrev, const Vector3& pi, const Vector3& pj) {
		return std::pow(Length(pj - pi), alpha) + tPrev;
		};

	float t0 = 0.0f;
	float t1 = tj(t0, p0, p1);
	float t2 = tj(t1, p1, p2);
	float t3 = tj(t2, p2, p3);

	// t を [t1, t2] の範囲にスケール
	float tt = std::lerp(t1, t2, std::clamp(t, 0.0f, 1.0f));

	// 線形補間で逐次計算（de Casteljau風）
	Vector3 A1 = ((t1 - tt) / (t1 - t0)) * p0 + ((tt - t0) / (t1 - t0)) * p1;
	Vector3 A2 = ((t2 - tt) / (t2 - t1)) * p1 + ((tt - t1) / (t2 - t1)) * p2;
	Vector3 A3 = ((t3 - tt) / (t3 - t2)) * p2 + ((tt - t2) / (t3 - t2)) * p3;

	Vector3 B1 = ((t2 - tt) / (t2 - t0)) * A1 + ((tt - t0) / (t2 - t0)) * A2;
	Vector3 B2 = ((t3 - tt) / (t3 - t1)) * A2 + ((tt - t1) / (t3 - t1)) * A3;

	Vector3 C = ((t2 - tt) / (t2 - t1)) * B1 + ((tt - t1) / (t2 - t1)) * B2;

	return C;
}

// 曲線全体の中での位置計算

static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t)
{
	//// 
	//assert(points.size() >= 4 && "制御点は4点以上必要です");


	//// 区間数は制御点の数-1
	//size_t division = points.size() - 1;

	//// 1区間の長さ
	//float areaWidth = 1.0f / division;

	//// 区間の始点を0.0f,終点を1.0fとしたときの現在地
	//float t_2 = std::fmod(t, areaWidth) * division;

	//t_2 = std::clamp(t_2, 0.0f, 1.0f);

	//// 区間のインデックス
	//size_t index = static_cast<size_t>(t / areaWidth);


	//// 制御点のインデックス
	//size_t index0 = index - 1;
	//size_t index1 = index;
	//size_t index2 = index + 1;
	//size_t index3 = index + 2;

	//// インデックスが範囲外の場合は、最初または最後の点を使用s
	//// 最初の区間のp0はp1を重複使用
	//if (index == 0)
	//{
	//	index0 = index1;
	//}

	//if (index >= points.size())
	//{
	//	index3 = index2;
	//}

	//// 4点の座標
	//const Vector3& p0 = points[index0];
	//const Vector3& p1 = points[index1];
	//const Vector3& p2 = points[index2];
	//const Vector3& p3 = points[index3];

	//// 4点を指定してCatmull-Rom補間
	//return CatmullRomInterpolation(p0, p1, p2, p3, t_2);

	// /////////////////////////////////////////////////////////////改良版1

	//assert(points.size() >= 4 && "制御点は4点以上必要です");

	//// 区間数は制御点の数 - 1
	//size_t division = points.size() - 1;

	//// 1区間の長さ
	//float areaWidth = 1.0f / division;

	//// 区間内の補間係数を計算
	//float t_2 = std::fmod(t, areaWidth) * division;
	//t_2 = std::clamp(t_2, 0.0f, 1.0f);

	//// 区間インデックス
	//size_t index = static_cast<size_t>(t / areaWidth);

	//// index が points.size() を超えるのを防ぐ
	//if (index >= points.size()) {
	//	index = points.size() - 1;
	//}

	//// 制御点インデックス計算（範囲外にならないよう clamp）
	//auto safeIndex = [&](int i) -> size_t {
	//	return static_cast<size_t>(std::clamp(i, 0, static_cast<int>(points.size() - 1)));
	//	};

	//size_t index0 = safeIndex(static_cast<int>(index) - 1);
	//size_t index1 = safeIndex(static_cast<int>(index));
	//size_t index2 = safeIndex(static_cast<int>(index) + 1);
	//size_t index3 = safeIndex(static_cast<int>(index) + 2);

	//// 4点の座標
	//const Vector3& p0 = points[index0];
	//const Vector3& p1 = points[index1];
	//const Vector3& p2 = points[index2];
	//const Vector3& p3 = points[index3];

	//// 補間実行
	//return CatmullRomInterpolation(p0, p1, p2, p3, t_2);


	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数（4点で1区間 → n-3）
	size_t numSegments = points.size() - 3;

	// t を [0, 1] にクランプ
	t = std::clamp(t, 0.0f, 1.0f);

	// 区間を計算
	float segmentT = t * static_cast<float>(numSegments);
	size_t i = static_cast<size_t>(segmentT);
	if (i >= numSegments) i = numSegments - 1;
	float localT = segmentT - static_cast<float>(i);

	// 対応する4点を取得
	const Vector3& p0 = points[i + 0];
	const Vector3& p1 = points[i + 1];
	const Vector3& p2 = points[i + 2];
	const Vector3& p3 = points[i + 3];

	// セントリペタルCatmull-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, localT);
}