#pragma once

namespace Kouro
{
	class ISceneState
	{
	public:
		/// \brief 仮想デストラクタ
		virtual ~ISceneState() = default;

		/// \brief シーン状態に入るときの処理
		virtual void OnEnter() = 0;

		/// \brief 状態の更新処理
		virtual void Update() = 0;

		/// \brief 状態から出るときの処理
		virtual void OnExit() = 0;
	};
}