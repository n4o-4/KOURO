#pragma once
#include "MyMath.h"

/**================================================================================
 * \brief  IsAABBCollision
 * \param  aabb1 AABB1（最小座標・最大座標）
 * \param  aabb2 AABB2（最小座標・最大座標）
 * \return 衝突していればtrue
 */
bool IsAABBCollision(const AABB& aabb1, const AABB& aabb2);

/**================================================================================
 * \brief  IsAABBSphereCollision
 * \param  aabb   AABB（最小座標・最大座標）
 * \param  sphere Sphere（中心・半径）
 * \return 衝突していればtrue
 */
bool IsAABBSphereCollision(const AABB& aabb, const Sphere& sphere);

/**================================================================================
 * \brief  IsSphereCollision
 * \param  sphere1 Sphere1（中心・半径）
 * \param  sphere2 Sphere2（中心・半径）
 * \return 衝突していればtrue
 */

bool IsSphereCollision(const Sphere& sphere1, const Sphere& sphere2);