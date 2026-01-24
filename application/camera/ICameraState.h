#pragma once
#include "WorldTransform.h"

namespace Kouro
{
	class BaseCamera;

}

class ICameraState
{
public:

	ICameraState(Kouro::BaseCamera* camera, const Kouro::WorldTransform* target) : camera_(camera), target_(target) {};

	/// \brief デストラクタ
	virtual ~ICameraState() = default;

	/// \brief 開始処理
	virtual void Enter() = 0;

	/// \brief 更新処理
	virtual void Update(float deltaTime) = 0;

	/// \brief 終了処理
	virtual void Exit() = 0;

protected:

	Kouro::BaseCamera* camera_ = nullptr; //!< 処理するカメラのポインタ

	const Kouro::WorldTransform* target_ = nullptr; //!< 視点のワールドトランスフォームのポインタ
};

