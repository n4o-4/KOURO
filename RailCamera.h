#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

class RailCamera : public BaseCamera
{
public:

	// ������
	void Initialize() override;

	// �X�V
	void Update() override;

private:

	std::vector<Vector3> controlPoints_; //!< �J�����̐���_

	const float kMoveTime = 10.0f;

	float moveTimer_ = 0.0f; //!< �J�����̈ړ�����

};

