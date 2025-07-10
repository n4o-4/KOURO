#pragma once
#include "MyMath.h"

/**================================================================================
 * \brief  IsCollision
 * \param  aabb1 AABB1（最小座標・最大座標）
 * \param  aabb2 AABB2（最小座標・最大座標）
 * \return 衝突していればtrue
 */
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

/**================================================================================
 * \brief  IsCollision
 * \param  sphere1 Sphere1（中心・半径）
 * \param  sphere2 Sphere2（中心・半径）
 * \return 衝突していればtrue
 */

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

/**================================================================================
 * \brief  IsCollision
 * \param  obb1 OBB1（中心・半径・ハーフサイズ）
 * \param  OBB2 OBB2（中心・半径・ハーフサイズ）
 * \return 衝突していればtrue
 */

bool IsCollision(const OBB& obb1, const OBB& obb2);

/**================================================================================
 * \brief  IsCollision
 * \param  aabb   AABB（最小座標・最大座標）
 * \param  sphere Sphere（中心・半径）
 * \return 衝突していればtrue
 */

bool IsCollision(const AABB& aabb, const Sphere& sphere);

/**================================================================================
 * \brief  IsCollision
 * \param  sphere Sphere（中心・半径）
 * \param  aabb   AABB（最小座標・最大座標）
 * \return 衝突していればtrue
 */

bool IsCollision(const Sphere& sphere, const AABB& aabb) { return IsCollision(aabb, sphere); }

/**================================================================================
 * \brief  IsCollision
 * \param  sphere Sphere（中心・半径）
 * \param  obb    OBB（中心・半径・ハーフサイズ）
 * \return 衝突していればtrue
 */

bool IsCollision(const Sphere& sphere, const OBB& obb);

/**================================================================================
 * \brief  IsCollision
 * \param  obb    OBB（中心・半径・ハーフサイズ）
 * \param  sphere Sphere（中心・半径）
 * \return 衝突していればtrue
 */

bool IsCollision(const OBB& obb, const Sphere& sphere) { return IsCollision(sphere, obb); }

/**================================================================================
 * \brief  IsCollision
 * \param  aabb AABB（最小座標・最大座標）
 * \param  obb  OBB（中心・半径・ハーフサイズ）
 * \return 衝突していればtrue
 */

bool IsCollision(const AABB& aabb, const OBB& obb);

/**================================================================================
 * \brief  IsCollision
 * \param  obb  OBB（中心・半径・ハーフサイズ）
 * \param  aabb AABB（最小座標・最大座標）
 * \return 衝突していればtrue
 */

bool IsCollision(const OBB& obb, const AABB& aabb) { return IsCollision(aabb, obb); }