#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "MyMath.h"

#include "ViewProjection.h"

class Camera
{
private:

	static std::unique_ptr<Camera> instance;

	friend std::unique_ptr<Camera> std::make_unique<Camera>();
	friend std::default_delete<Camera>;

	Camera() = default;
	~Camera() = default;
	Camera(Camera&) = delete;
	Camera& operator=(Camera&) = delete;


public:

	static Camera* GetInstance();

	void Initialize();

	void Finalize();

	// 更新
	void Update();

	// setter
	void SetRotate(const Vector3& rotate) { viewProjection.transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { viewProjection.transform.translate = translate; }
	void SetFovY(float fovY) { viewProjection.fovY = fovY; }
	void SetAspectRation(float aspectRation) { viewProjection.aspectRation = aspectRation; }

	// getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }
	/*const Matrix4x4& GetViewMatrix() const { return viewProjection.viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return viewProjection.projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }*/
	const Vector3& GetRotate() const { return viewProjection.transform.rotate; }
	const Vector3& GetTranslate() const { return viewProjection.transform.translate; }

	ViewProjection& GetViewProjection() { return viewProjection; }

private:
	
	ViewProjection viewProjection;

	Matrix4x4 worldMatrix;

	/*float fovY;
	float aspectRation;
	float nearClip;
	float farClip;*/
};

