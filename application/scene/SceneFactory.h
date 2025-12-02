#pragma once

#include "AbstractSceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

#include "GameClear.h"
#include "GameOver.h"

// \brief SceneFactory
// シーン生成用のファクトリクラス。
// AbstractSceneFactoryを継承し、シーン名に応じたBaseScene派生クラスのインスタンスを生成する。
// ゲームのシーン切り替え時に利用される。

class SceneFactory : public AbstaractSceneFactory
{
public:

	/**
	* \brief  CreateScene シーンの生成	
	* \param  string シーンの名前
	* \return BaseScene 生成したシーンの生ポインタ
	*/
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;

};