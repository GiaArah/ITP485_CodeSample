#include "stdafx.h"
#include "SkinnedObj.h"
#include "Graphics.h"

SkinnedObj::SkinnedObj(const Mesh* mesh) : RenderObj(mesh)
{
	// fill skinning matrices with identity matrices
	for (int i = 0; i < maxMatrices; i++)
	{
		mSkinConstants.c_skinMatrix[i] = Matrix4::Identity;
	}

	// initialize buffer
	Graphics* pGraphics = Graphics::Get();
	mSkinnedBuffer = pGraphics->CreateGraphicsBuffer(&mSkinConstants.c_skinMatrix, sizeof(mSkinConstants.c_skinMatrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

}

SkinnedObj::~SkinnedObj()
{
	mSkinnedBuffer->Release();
}

void SkinnedObj::Draw()
{
	// Upload the object data to the buffers, set the buffers, and draw
	Graphics* pGraphics = Graphics::Get();
	pGraphics->UploadBuffer(mSkinnedBuffer, &mSkinConstants.c_skinMatrix, sizeof(mSkinConstants.c_skinMatrix));
	ID3D11DeviceContext* pDeviceContext = pGraphics->GetDeviceContext();
	pDeviceContext->VSSetConstantBuffers(4, 1, &mSkinnedBuffer);

	RenderObj::Draw();
}

void SkinnedObj::UploadBufferData(std::vector<Matrix4>& bonePoseData)
{
	for (int i = 0; i < bonePoseData.size(); i++)
	{
		mSkinConstants.c_skinMatrix[i] = bonePoseData[i];
	}
}
