#include "CollisionMath.h"

bool IsAABBCollision(const AABB& aabb1, const AABB& aabb2)
{
	// 各軸で重なりがあるか判定
	bool overlapX = (aabb1.min.x + aabb1.center.x <= aabb2.max.x + aabb2.center.x)
		&& (aabb1.max.x + aabb1.center.x >= aabb2.min.x);
	bool overlapY = (aabb1.min.y + aabb1.center.y <= aabb2.max.y + aabb2.center.y)
		&& (aabb1.max.y + aabb1.center.y >= aabb2.min.y + aabb2.center.y);
	bool overlapZ = (aabb1.min.z + aabb1.center.x <= aabb2.max.z + aabb2.center.z)
		&& (aabb1.max.z + aabb1.center.z >= aabb2.min.z + aabb2.center.z);

	// すべての軸で重なっていれば衝突
	return overlapX && overlapY && overlapZ;
}

bool IsAABBSphereCollision(const AABB& aabb, const Sphere& sphere)
{
	// AABB内でSphere中心に最も近い点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x, aabb.min.x + aabb.center.x, aabb.max.x + aabb.center.x),
		std::clamp(sphere.center.y, aabb.min.y + aabb.center.y, aabb.max.y + aabb.center.y),
		std::clamp(sphere.center.z, aabb.min.z + aabb.center.z, aabb.max.z + aabb.center.z)
	};

	// 最近接点と球の中心の距離を計算
	float dx = closestPoint.x - sphere.center.x;
	float dy = closestPoint.y - sphere.center.y;
	float dz = closestPoint.z - sphere.center.z;
	float distanceSq = dx * dx + dy * dy + dz * dz;

	// 距離の2乗が半径の2乗以下なら衝突
	return distanceSq <= (sphere.radius * sphere.radius);
}

bool IsSphereCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2つの中心間の距離を計算
	float dx = sphere1.center.x - sphere2.center.x;
	float dy = sphere1.center.y - sphere2.center.y;
	float dz = sphere1.center.z - sphere2.center.z;
	float distanceSq = dx * dx + dy * dy + dz * dz;

	// 半径の和の2乗
	float radiusSum = sphere1.radius + sphere2.radius;
	float radiusSumSq = radiusSum * radiusSum;

	// 距離の2乗が半径の和の2乗以下なら衝突
	return distanceSq <= radiusSumSq;
}