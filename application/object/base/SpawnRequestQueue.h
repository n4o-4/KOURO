#pragma once
/// Application
#include "BaseBullet.h"
/// Engine
#include "Vector3.h"
/// Standard Library
#include <string>
#include <vector>
#include <functional>

class SpawnRequestQueue
{
public:
	struct BulletSpawnInfo
	{
		std::function<std::unique_ptr<BaseBullet>()> creator; //!< 弾生成関数
		std::string modelName; //!< モデル名
		Kouro::Vector3 position; //!< 生成位置
		Kouro::Vector3 velocity; //!< 速度
		uint32_t damage; //!< ダメージ量
	};
public:

	/**
	* \brief  生成要求をキューに追加する
	* \param  spawnInfo 生成要求情報
	*/
	void Push(const BulletSpawnInfo& spawnInfo);

	/**
	* \brief  生成要求を消費する
	* \return 生成要求のリスト
	*/
	std::vector<BulletSpawnInfo> Consume();

private:
	std::vector<BulletSpawnInfo> bulletSpawnRequests_; //!< 生成要求キュー
};
