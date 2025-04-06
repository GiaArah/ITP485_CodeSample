#pragma once
#include "engineMath.h"

struct alignas(16) PerCameraConstants
{
	Matrix4 c_viewProj;
	Vector3 c_cameraPosition;
};

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetActive();
	void SetViewMatrix(Matrix4 view);

private:
	PerCameraConstants mCameraConstants = {};
	ID3D11Buffer* mConstantBuffer = nullptr;

	Matrix4 mWorldToCamera = Matrix4::Identity;
	Matrix4 mProjection = Matrix4::Identity;
};

