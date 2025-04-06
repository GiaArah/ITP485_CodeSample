#include "stdafx.h"
#include "Camera.h"
#include "Graphics.h"

Camera::Camera()
{
	// initialize camera constants to zero (for now)
	mCameraConstants = {};

	Graphics* pGraphics = Graphics::Get();
	mWorldToCamera = Matrix4::CreateTranslation(Vector3(500.0f, 0.0f, 0.0f));
	mProjection = Matrix4::CreateRotationY(-Math::PiOver2)
		* Matrix4::CreateRotationZ(-Math::PiOver2)
		* Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
			pGraphics->GetScreenWidth(), pGraphics->GetScreenHeight(),
			25.0f, 10000.0f);

	mConstantBuffer = pGraphics->CreateGraphicsBuffer(&mCameraConstants, sizeof(mCameraConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Camera::~Camera()
{
	mConstantBuffer->Release();
}

void Camera::SetActive()
{
	// Set the view projection matrix as world-to-camera * projection matrix
	mCameraConstants.c_viewProj = mWorldToCamera * mProjection;

	// Set the camera's position as the transform from the inverse of the world-to-camera 
	Matrix4 cameraToWorld = mWorldToCamera;
	cameraToWorld.Invert();
	Vector3 cameraPos = cameraToWorld.GetTranslation();
	mCameraConstants.c_cameraPosition = cameraPos;

	// Upload buffer to vertex shader and pixel shader with camera constants
	Graphics* pGraphics = Graphics::Get();
	ID3D11DeviceContext* pDeviceContext = pGraphics->GetDeviceContext();
	pGraphics->UploadBuffer(mConstantBuffer, &mCameraConstants, sizeof(mCameraConstants));
	pDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &mConstantBuffer);
}

void Camera::SetViewMatrix(Matrix4 view)
{
	mWorldToCamera = view;
}
