#pragma once
#include "LineModel.h"

class LineModelManager
{
public:

	void Initialize(LineDrawerBase* lineDrawerBase);

	void LoadLineModel(std::string filePath);

	LineModel* FindLineModel(const std::string filePath);
private:

	LineDrawerBase* lineDrawerBase_ = nullptr;

	// モデルデータ
	std::map<std::string, std::unique_ptr<LineModel>> models_;

};

