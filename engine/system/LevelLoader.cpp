#include "LevelLoader.h"  

namespace Kouro
{
	std::unique_ptr<LevelLoader::LevelData> LevelLoader::LoadLevel(nlohmann::json deserialized)
	{

		// レベルデータ格納用インスタンスを生成
		std::unique_ptr<LevelLoader::LevelData> levelData = std::make_unique<LevelData>();

		// "objects"の全オブジェクトを走査
		for (nlohmann::json& object : deserialized["objects"])
		{
			assert(object.contains("type"));

			// 種類を取得
			std::string type = object["type"].get<std::string>();

			// Mesh
			if (type.compare("MESH") == 0)
			{
				// 要素追加
				levelData->objects.emplace_back(LevelLoader::ObjectData{});

				// 今追加した要素の参照を取得
				LevelLoader::ObjectData& objectData = levelData->objects.back();

				if (object.contains("name"))
				{
					// ファイル名
					objectData.fileName = object["name"];

					objectData.worldTransform = std::make_unique<WorldTransform>();

					objectData.worldTransform->Initialize();

					// トランスフォームのパラメータ読み込み
					nlohmann::json& transform = object["transform"];

					// 平行移動
					objectData.worldTransform->SetTranslate({ (float)transform["translate"][1], (float)transform["translate"][2], -(float)transform["translate"][0] });


					constexpr float kDeg2Rad = static_cast<float>(std::numbers::pi / 180.0f);

					objectData.worldTransform->SetRotate({ (float)transform["rotate"][1] * kDeg2Rad, (float)transform["rotate"][2] * kDeg2Rad, (float)transform["rotate"][0] * kDeg2Rad - static_cast<float>(std::numbers::pi / 2.0f) });

					// スケーリング
					objectData.worldTransform->SetScale({ (float)transform["scale"][1], (float)transform["scale"][2], (float)transform["scale"][0] });

					//==================================================
					/// object3D
					//==================================================

					// ファイル名から登録済みモデルを検索
					Model* model = ModelManager::GetInstance()->FindModel(object["file_name"]);

					objectData.object3d = std::make_unique<Object3d>();

					objectData.object3d->Initialize(Object3dCommon::GetInstance());

					objectData.object3d->SetModel(model);
				}

				if (object.contains("children"))
				{
					LoadLevel(deserialized);
				}
			}
		}

		return levelData;
	}

	std::unique_ptr<LevelLoader::LevelData> LevelLoader::LoadLevelFromJson(std::string fileName)
	{
		// 連結してフルパスを得る  
		const std::string fullpath = fileName;

		// ファイルストリーム  
		std::ifstream file;

		// ファイルを開く
		file.open(fullpath);

		// ファイルオープン失敗をチェック
		if (file.fail())
		{
			assert(0);
		}

		// JSON データを格納するオブジェクト
		nlohmann::json deserialized;

		// 解凍
		file >> deserialized;

		// 正しいレベルデータファイルかチェック
		assert(deserialized.is_object());
		assert(deserialized.contains("name"));
		assert(deserialized["name"].is_string());

		// "name"を文字列として取得
		std::string name = deserialized["name"].get<std::string>();

		// 正しいレベルファイルかチェック
		assert(name.compare("scene") == 0);

		return LoadLevel(deserialized);
	}
}