#pragma once
#include <memory>
#include <vector>

#include "MyMath.h"
#include "WorldTransform.h"

class RailAnimation
{
public: // メンバ構造体

	struct ArcLengthTable
	{
		std::vector<float> lengths; // 各セグメントの長さ
		float totalLength;          // 全体の長さ
	};

public: // メンバ関数

	void Initialize(std::vector<Vector3> controlPoints, int samplePerSegment = 100);

	WorldTransform* GetWorldTransform(const float progress);

private: // メンバ関数

	void CreateArcLengthTable(int samplePerSegment);

private: // メンバ変数

	std::vector<Vector3> controlPoints_; //!< コントロールポイントのリスト

	ArcLengthTable arcLengthTable_; //!< 弧長データ

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr; //!< ワールドトランスフォーム

	float offsetT_ = 0.05f;
};

