#pragma once
#include "Input.h"
#include "BaseCamera.h"

class DebugCamera : public BaseCamera
{
public:

	// ‰Šú‰»
	void Initialize() override;

	// XV
	void Update() override;

	ViewProjection& GetViewProjection() { return *viewProjection_; }

private:
};

