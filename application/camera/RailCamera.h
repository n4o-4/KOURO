#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

#include "WorldTransform.h"

#include "RailFollower.h"

// \brief  RailCamera レールカメラ

class RailCamera : public BaseCamera
{
public:

	/**
	* \brief  Initialize 初期化
	*/
	void Initialize() override;

	/**
	* \brief  Update 更新
	*/
	void Update() override;

	/**
	* \brief  GetWorldTransform ワールド変換情報を取得する
	* \return worldTransform 
	*/
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }
	
	/**
	* \brief  SetIsMove 移動フラグを設定する
	*/
	void SetIsMove(bool isMove) { isMove_ = isMove; }

	/**
	* \brief  
	* \param  rail     : 使用するレール
	* \param  moveTime : レールの移動にかかる時間(指定しなければ60秒)
	*/
	void SetRail(const Rail& rail,float moveTime = kDefaultMoveTime);

private:

	// ワールド変換情報
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	
	bool isMove_ = false;

	std::unique_ptr<RailFollower> railFollower_ = nullptr;

private:

	static constexpr float kDefaultMoveTime = 60.0f; // デフォルトの一周にかかる時間

};
