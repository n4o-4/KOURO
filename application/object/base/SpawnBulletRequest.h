#pragma once

#include "Vector3.h"

struct SpawnBulletRequest
{
	Kouro::Vector3 position = { 0.0f, 0.0f, 0.0f }; // 弾の発射位置
	Kouro::Vector3 velocity = { 0.0f, 0.0f, 0.0f }; // 弾の速度
};