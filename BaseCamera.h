#pragma once

#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"	
#include "ViewProjection.h"	

class BaseCamera
{
public:
	// ‰Šú‰»
	virtual void Initialize();

	// XVˆ—
	virtual void Update();

private:

	ViewProjection viewProjection;

	Matrix4x4 worldMatrix;

};

