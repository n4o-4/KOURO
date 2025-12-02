#pragma once
#include "EnemyState.h"

// \brief ApproachState 接近状態クラス

class ApproachState : public EnemyState
{
	public:
		/**
		* \brief  Enter 
		* \param  Enemy
		*/
		void Enter(Enemy* enemy) override;

		/**
		* \brief  Update �X�V
		* \param  Enemy
		*/
		void Update(Enemy* enemy) override;

		/**
		* \brief  Exit  
		* \param  Enemy 
		*/
		void Exit(Enemy* enemy) override;
};