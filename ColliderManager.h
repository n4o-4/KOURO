#pragma once
#include "Kouro.h"

#include "BaseCollider.h"

class ColliderManager
{
public:  /// ���J�����o�֐�
	
	
	// \brief  Update �X�V

	void Update();


private: /// ����J�����o�֐�
public:  /// ���J�����o�ϐ�
private: /// ����J�����o�ϐ�

	std::vector<BaseCollider*> colliders_; // �R���C�_�[�̃��X�g
	
};