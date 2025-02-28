#pragma once
#include "Camera.h"

class Player;

class FollowCamera {
public:
	void Initialize();
	void Update(Player* player);

private:
	Vector3 offset_ = { 0.0f, 2.0f, -30.0f }; // カメラのプレイヤーからのオフセット
};
