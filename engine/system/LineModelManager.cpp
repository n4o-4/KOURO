#include "LineModelManager.h"

namespace Kouro
{
	void LineModelManager::Initialize(LineDrawerBase* lineDrawerBase)
	{
		lineDrawerBase_ = lineDrawerBase;
	}

	void LineModelManager::LoadLineModel(std::string filePath)
	{
		// 読み込み済みモデル検索
		if (models_.contains(filePath)) {
			return;
		}

		// モデルの生成とファイル読み込み、初期化
		std::unique_ptr<LineModel> model = std::make_unique<LineModel>();
		model->Initialize(lineDrawerBase_, filePath);

		// モデルをmapコンテナに格納する
		models_.insert(std::make_pair(filePath, std::move(model)));
	}

	LineModel* LineModelManager::FindLineModel(const std::string filePath)
	{
		// 読み込み済みモデルを検索
		if (models_.contains(filePath)) {
			// 読み込みモデルを戻り値としてreturn
			return models_.at(filePath).get();
		}

		assert(0);
		return nullptr;
	}
}