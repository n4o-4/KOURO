#include "SpawnRequestQueue.h"

void SpawnRequestQueue::Push(const BulletSpawnInfo& spawnInfo)
{
	bulletSpawnRequests_.push_back(spawnInfo);
}

std::vector<SpawnRequestQueue::BulletSpawnInfo> SpawnRequestQueue::Consume()
{
	std::vector<BulletSpawnInfo> requests = std::move(bulletSpawnRequests_);
	bulletSpawnRequests_.clear();
	return requests;
}