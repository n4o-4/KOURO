#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include "Structs.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp\postprocess.h>

#include "Model.h"

// \brief AnimationManager
// モデルのアニメーション管理を行うシングルトンクラス。
// キーフレームベースでの位置・回転・スケールの補間、ループ再生、スケルトン更新をサポート。

namespace Kouro
{
	class AnimationManager
	{
	private:
		template <typename tValue>
		struct Keyframe
		{
			float time; // キーフレームの時刻
			tValue value; // キーフレームの値
		};
		using KeyframeVector3 = Keyframe<Vector3>;
		using KeyframeQuaternion = Keyframe<Quaternion>;

		template <typename tValue>
		struct AnimationCurve
		{
			std::vector<Keyframe<tValue>> keyframes;
		};

		struct NodeAnimation
		{
			AnimationCurve<Vector3> translate;
			AnimationCurve<Quaternion> rotate;
			AnimationCurve<Vector3> scale;
		};

		struct Animation
		{
			float duration = 0.0f; // アニメーション全体の尺(秒)

			// NodeAnimationの集合体
			std::unordered_map<std::string, NodeAnimation> nodeAnimations;
			ModelData modelData;

			Skeleton skeleton;
		};

		struct AnimationState
		{
			Animation animation;
			int maxLoopCount = 0;
			int currentLoopCount = 0;
			float animationTime = 0.0f;
			bool isInfiniteLoop = 0;
		};

		///------------------------------
		/// メンバ関数
		///------------------------------
	public:

		/// \brief 更新
		void Update();

		/**
		* \brief  アニメーションファイルの読み込み
		* \param  directoryPath ディレクトリパス
		* \param  filename ファイル名
		*/
		void LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

		/**
		* \brief  モデルファイルの読み込み
		* \param  directoryPath ディレクトリパス
		* \param  filename ファイル名
		* \return 読み込んだモデルデータ
		*/
		ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

		/**
		* \brief  アニメーションの開始
		* \param  filename アニメーションファイル名
		* \param  loopCount ループ回数（0で無限ループ）
		*/
		void StartAnimation(const std::string& filename, int loopCount);

		/// \brief アニメーションの再生
		void PlayAnimation();

		/**
		* \brief  ローカル行列の取得
		* \return ローカル行列
		*/
		Matrix4x4 GetLocalMatrix() { return localMatrix; }

		/**
		* \brief  スケルトンの作成
		* \param  rootNode ルートノード
		* \return 作成したスケルトン
		*/
		Skeleton CreateSkeleton(const Node& rootNode);

		/**
		* \brief  ジョイントの作成
		* \param  node ノード情報
		* \param  parent 親ジョイントのIndex
		* \param  joints ジョイントの配列
		* \return 作成したジョイントのIndex
		*/	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

		/**
		* \brief  スケルトンの更新
		* \param  skeleton 更新するスケルトン
		*/
		void SkeletonUpdate(Skeleton& skeleton);

		/**
		* \brief  アニメーションの取得
		* \param  filename アニメーションファイル名
		* \return 取得したアニメーション
		*/
		Animation GetAnimation(const std::string& filename) { return animations[filename]; }

		/**
		* \brief  アクティブなアニメーションの取得
		* \param  filename アニメーションファイル名
		* \return 取得したアクティブなアニメーション
		*/
		Animation GetActiveAnimation(const std::string& filename) { return activeAnimations[filename].animation; }

	private:

		/**
		* \brief  キーフレームからVector3値を計算
		* \param  keyframes キーフレームの配列
		* \param  time 時刻
		* \return 計算したVector3値
		*/
		Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);

		/**
		* \brief  キーフレームからQuaternion値を計算
		* \param  keyframes キーフレームの配列
		* \param  time 時刻
		* \return 計算したQuaternion値
		*/
		Quaternion CalculateQuaternion(const std::vector<KeyframeQuaternion>& keyframes, float time);

		///------------------------------
		/// メンバ変数
		///------------------------------
	public:

		std::unordered_map<std::string, Animation> animations;

		std::unordered_map<std::string, AnimationState> activeAnimations;

	private:

		Matrix4x4 localMatrix = MakeIdentity4x4();

		const float deltaTime = 1.0f / 60.0f;

		float animationTime_ = 0.0f;

		int loopCount_ = 0;
	};
}