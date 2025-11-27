#pragma once
#include "LineModel.h"

// \brief LineModelManager
// ラインモデルを管理するクラス。
// モデルの読み込み、検索、および内部保持を行う。
// 内部でstd::mapを用いてファイルパスとLineModelの対応を管理。

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

