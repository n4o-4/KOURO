#pragma once
/// standard library
#include <memory>

/// engine
#include "ISceneState.h"

/// game

class GameStateMachine
{
public:

	void Initialize();
	void Update();

private:

	std::unique_ptr<Kouro::ISceneState> currentState_;
	std::unique_ptr<Kouro::ISceneState> nextState_;

};

