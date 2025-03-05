#pragma once
#include "Vectors.h"
#include "Object3d.h"
#include "WorldTransform.h"
#include "BaseObject.h"
#include "Enemy.h"

///=============================================================================
///						プレイヤーの弾クラス
class PlayerBullet : public BaseObject {
	///--------------------------------------------------------------
	///							メンバ関数
public:
	/**----------------------------------------------------------------------------
	 * \brief  PlayerBullet コンストラクタ
	 * \param  position 位置
	 * \param  velocity 速度
	 * \param  scale 大きさ
	 * \param  rotate 回転
	 */
	PlayerBullet(Vector3 position, Vector3 velocity, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 rotate = { 0.0f, 0.0f, 0.0f });
	/**----------------------------------------------------------------------------
	* \brief  Initialize 初期化関数
	*/
	void Initialize(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 rotate); // 初期化関数

	/**----------------------------------------------------------------------------
	 * \brief  Update 更新
	 */
	void Update();
	/**----------------------------------------------------------------------------
	 * \brief  Draw 描画
	 * \param  viewProjection ビュー射影行列
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	///--------------------------------------------------------------
	///							静的メンバ関数
private:

	///--------------------------------------------------------------
	///						 当たり判定
	/// \brief 衝突開始時の処理
	void OnCollisionEnter(BaseObject *other) override;

	/// \brief 衝突継続時の処理
	void OnCollisionStay(BaseObject *other) override;

	/// \brief 衝突終了時の処理
	void OnCollisionExit(BaseObject *other) override;

	///--------------------------------------------------------------
	///						 入出力関数
public:
	/**----------------------------------------------------------------------------
	* \brief  OnCollisionEnter 衝突開始時の処理
	* \param  other 衝突相手
	*/
	bool IsActive() const { return isActive_; }
	/**----------------------------------------------------------------------------
	 * \brief  GetPosition 位置を取得
	 * \return 位置
	 */
	Vector3 GetPosition() const { return objectTransform_->transform.translate; }
	/**----------------------------------------------------------------------------
	 * \brief  SetScale 大きさを設定
	 * \param  scale
	 */
	void SetScale(Vector3 scale) { objectTransform_->transform.scale = scale; }
	/**----------------------------------------------------------------------------
	 * \brief  SetRotation
	 * \param  rotation
	 */
	void SetRotation(Vector3 rotation) { objectTransform_->transform.rotate = rotation; }
	/**----------------------------------------------------------------------------
	 * \brief  SetPosition
	 * \param  position
	 */
	void SetPosition(Vector3 position) { objectTransform_->transform.translate = position; }

	///--------------------------------------------------------------
	///						 メンバ変数
private:
	//========================================
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;
	std::unique_ptr<WorldTransform> objectTransform_; // `objectTransform_` に変更

	//========================================
	// 速度
	Vector3 velocity_;
	// 大きさ
	bool isActive_ = true;
};
