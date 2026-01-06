#pragma once
#include <random>
#include "Vectors.h"
class RandomGenerate
{
public:

	/// \brief 初期化
	void Initialize();

	/**
	* \brief  
	*/
	float Random(float min, float max);

	Vector3 RandomVector3(float min, float max);

private:

	std::mt19937 randomEngine;

};

