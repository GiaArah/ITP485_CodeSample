#include "stdafx.h"
#include "RenderObj.h"
#include "VertexBuffer.h"
#include "Graphics.h"
#include "Material.h"
#include "mesh.h"
#include "Component.h"

RenderObj::RenderObj(const Mesh* mesh)
{
	// initialize the object buffer and set initial data to send to buffer
	mObjectConstants.c_modelToWorld = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
	Graphics* pGraphics = Graphics::Get();
	mObjectBuffer = pGraphics->CreateGraphicsBuffer(&mObjectConstants.c_modelToWorld, sizeof(mObjectConstants.c_modelToWorld), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	mMesh = mesh;
}

RenderObj::~RenderObj()
{
	mObjectBuffer->Release();
	for (Component* comp : mComponents)
	{
		delete(comp);
	}
}

void RenderObj::Draw()
{
	// Upload the object data to the buffers, set the buffers, and draw
	Graphics* pGraphics = Graphics::Get();
	pGraphics->UploadBuffer(mObjectBuffer, &mObjectConstants.c_modelToWorld, sizeof(mObjectConstants.c_modelToWorld));
	ID3D11DeviceContext* pDeviceContext = pGraphics->GetDeviceContext();
	pDeviceContext->VSSetConstantBuffers(1,1,&mObjectBuffer);

	mMesh->Draw();
}

void RenderObj::Update(float deltaTime)
{
	for (Component* comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void RenderObj::AddComponent(Component* pComp)
{
	mComponents.push_back(pComp);
}
