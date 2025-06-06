#include "LevelLoader.h"  


void LevelLoader::LoadLevel(std::string fileName)
{
	// �A�����ăt���p�X�𓾂�  
	const std::string fullpath = fileName;

	// �t�@�C���X�g���[��  
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullpath);

	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	// JSON �f�[�^���i�[����I�u�W�F�N�g
	nlohmann::json deserialized;
	
	// ��
	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	// ���������x���t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);
}
