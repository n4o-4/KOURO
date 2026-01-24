#pragma once

#include "WorldTransform.h"

constexpr int kCatmullRomPointOffset = 3; //!< Catmull-Romスプラインの有効区間補正値
constexpr int kIncludeStartPoint = 1;     //!< サンプル数に始点を含める補正値

namespace RailData
{
	struct ArcLengthTable
	{
		std::vector<float> lengths; // 各セグメントの長さ

		float totalLength = 0.0f;          // 全体の長さ
	};
}

// \brief Rail
// 3次ベジェ曲線やスプラインに基づくカメラやオブジェクトの軌道管理クラス。
// 制御点（controlPoints）から曲線を生成し、距離に基づく移動をサポート。
// ArcLengthTableを用いて移動距離と時間の補間を行い、WorldTransformを通じて位置・向きを取得可能。

class Rail
{
public:

	/**
	* \brief  初期化
	* \param  controlPoints    : コントロールポイントのリスト
	* \param  samplePerSegment : サンプリングの数
	*/
	void Initialize(const std::vector<Kouro::Vector3>& controlPoints, int samplePerSegment = 100);

	/**
	* \brief  弧長データを取得する
	* \return 弧長データ
	*/
	const RailData::ArcLengthTable& GetArcLengthTable() const { return arcLengthTable_; }

	/**
	* \brief  コントロールポイントのリストを取得する
	* \return コントロールポイントのリスト
	*/
	const std::vector<Kouro::Vector3>& GetControlPoints() const { return controlPoints_; }

private:

	void CreateArcLengthTable(int samplePerSegment);

private:

	std::vector<Kouro::Vector3> controlPoints_; //!< コントロールポイントのリスト

	RailData::ArcLengthTable arcLengthTable_; //!< 弧長データ
};

