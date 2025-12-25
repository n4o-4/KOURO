#include "CollisionMath.h"

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
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

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
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

bool IsCollision(const OBB& obb1, const OBB& obb2)
{
	// OBBの中心間のベクトルを計算
	Vector3 centerDiff = {
		obb1.center.x - obb2.center.x,
		obb1.center.y - obb2.center.y,
		obb1.center.z - obb2.center.z
	};

	// 各軸での投影を計算
	for (int i = 0; i < 3; ++i) {
		float projection1 = Dot(centerDiff, obb1.axes[i]) + Dot(obb1.halfSize, obb1.axes[i]);
		float projection2 = Dot(centerDiff, obb2.axes[i]) + Dot(obb2.halfSize, obb2.axes[i]);
		if (std::abs(projection1) > std::abs(projection2)) {
			return false; // 衝突しない
		}
	}

	return true; // 衝突している
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
    // AABBのワールド空間でのmin/maxを求める
    Vector3 aabbMinWorld = aabb.min + aabb.center;
    Vector3 aabbMaxWorld = aabb.max + aabb.center;

    // AABB内でSphere中心に最も近い点を求める
    Vector3 closestPoint{
        std::clamp(sphere.center.x, aabbMinWorld.x, aabbMaxWorld.x),
        std::clamp(sphere.center.y, aabbMinWorld.y, aabbMaxWorld.y),
        std::clamp(sphere.center.z, aabbMinWorld.z, aabbMaxWorld.z)
    };

    // 最近接点と球の中心の距離を計算
    Vector3 diff = closestPoint - sphere.center;
    float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    // 距離の2乗が半径の2乗以下なら衝突
    return distanceSq <= (sphere.radius * sphere.radius);
}

bool IsCollision(const Sphere& sphere, const AABB& aabb)
{
    return IsCollision(aabb, sphere);
}

bool IsCollision(const Sphere& sphere, const OBB& obb)
{
    Vector3 d = sphere.center - obb.center;
    Vector3 closest = obb.center;

    // 各軸方向へ投影し、範囲内に制限
    for (int i = 0; i < 3; ++i) {
        float dist = Dot(d, obb.axes[i]);

        float extent = 0.0f;
        if (i == 0) extent = obb.halfSize.x;
        else if (i == 1) extent = obb.halfSize.y;
        else if (i == 2) extent = obb.halfSize.z;

        dist = std::max(-extent, std::min(dist, extent));
        closest = closest + obb.axes[i] * dist;
    }

    Vector3 v = closest - sphere.center;
    return LengthSq(v) <= sphere.radius * sphere.radius;
}

bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	return IsCollision(sphere, obb);
}

bool IsCollision(const AABB& aabb, const OBB& obb)
{
    // AABBの中心と半サイズを計算
    Vector3 aabbHalfSize = (aabb.max - aabb.min) * 0.5f;
    Vector3 aabbCenter = aabb.min + aabbHalfSize;

    // AABBのローカル軸（ワールド空間で固定）
    Vector3 aabbAxes[3] = {
        {1.0f, 0.0f, 0.0f}, // X
        {0.0f, 1.0f, 0.0f}, // Y
        {0.0f, 0.0f, 1.0f}  // Z
    };

    const Vector3* obbAxes = obb.axes;

    Vector3 d = obb.center - aabbCenter;

    // 分離軸リスト（15個）
    Vector3 axesToTest[15];
    int axisCount = 0;

    // AABBの軸
    for (int i = 0; i < 3; ++i) {
        axesToTest[axisCount++] = aabbAxes[i];
    }

    // OBBの軸
    for (int i = 0; i < 3; ++i) {
        axesToTest[axisCount++] = obbAxes[i];
    }

    // AABB × OBB の交差積軸（9軸）
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            Vector3 axis = {
                aabbAxes[i].y * obbAxes[j].z - aabbAxes[i].z * obbAxes[j].y,
                aabbAxes[i].z * obbAxes[j].x - aabbAxes[i].x * obbAxes[j].z,
                aabbAxes[i].x * obbAxes[j].y - aabbAxes[i].y * obbAxes[j].x
            };
            // 軸がゼロでないか（平行な軸のクロス積はゼロになる）
            float lenSq = LengthSq(axis);
            if (lenSq > 1e-5f) {
                // 正規化
                float len = std::sqrt(lenSq);
                axis = axis * (1.0f / len);
                axesToTest[axisCount++] = axis;
            }
        }
    }

    // 全軸で分離軸チェック
    for (int i = 0; i < axisCount; ++i) {
        const Vector3& axis = axesToTest[i];

        // AABBの投影半径
        float rAABB =
            aabbHalfSize.x * std::fabs(Dot(axis, aabbAxes[0])) +
            aabbHalfSize.y * std::fabs(Dot(axis, aabbAxes[1])) +
            aabbHalfSize.z * std::fabs(Dot(axis, aabbAxes[2]));

        // OBBの投影半径
        float rOBB =
            obb.halfSize.x * std::fabs(Dot(axis, obbAxes[0])) +
            obb.halfSize.y * std::fabs(Dot(axis, obbAxes[1])) +
            obb.halfSize.z * std::fabs(Dot(axis, obbAxes[2]));

        // 中心間距離の軸への投影
        float distance = std::fabs(Dot(d, axis));

        if (distance > rAABB + rOBB) {
            // 分離軸があったら当たっていない
            return false;
        }
    }

    // 全軸で分離できなかった → 衝突している
    return true;
}

bool IsCollision(const OBB& obb, const AABB& aabb)
{
	return IsCollision(aabb, obb);
}