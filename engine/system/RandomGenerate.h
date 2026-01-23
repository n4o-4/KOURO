#pragma once
#include <random>
#include "Vectors.h"

/// \brief ランダム生成クラス

namespace Kouro
{
	class RandomGenerate
	{
	public:

		/// \brief 初期化
		void Initialize();

		/**
		* \brief  ランダムなfloat値を生成する
		* \param  min : 最小値
		* \param  max : 最大値
		*/
		float Random(float min, float max);

		/**
		* \brief  ランダムな3Dベクトルを生成する
		* \param  min : 最小値
		* \param  max : 最大値
		*/
		Vector3 RandomVector3(float min, float max);

	private:

		std::mt19937 randomEngine;

	};
}