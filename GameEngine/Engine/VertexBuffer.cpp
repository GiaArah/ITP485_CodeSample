#include "stdafx.h"
#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::VertexBuffer(
	const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
	const void* indexData, uint32_t indexCount, uint32_t indexStride
)
{
	// infer the index buffer format from the stride
	if (indexStride == 2) // 16-bit
	{
		mIndexBufferFormat = DXGI_FORMAT_R16_UINT;
	}
	else if (indexStride == 4) // 32-bit
	{
		mIndexBufferFormat = DXGI_FORMAT_R32_UINT;
	}
	else // handle invalid index stride
	{
		DbgAssert(false, "Invalid index stride size. Must be 16-bit or 32-bit");
	}
	mVertStride = vertexStride;
	mIndexCount = indexCount;

	// Create vertex buffer
	Graphics* pGraphics = Graphics::Get();
	mVertexBuffer = pGraphics->CreateGraphicsBuffer(vertexData, (vertexStride*vertexCount), D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
	
	// Create index buffer
	mIndexBuffer = pGraphics->CreateGraphicsBuffer(indexData, (indexStride * indexCount), D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

VertexBuffer::~VertexBuffer()
{
	mVertexBuffer->Release();
	mIndexBuffer->Release();
}

void VertexBuffer::Draw() const
{
	Graphics* pGraphics = Graphics::Get();
	uint32_t pOffsets = 0;

	ID3D11DeviceContext* pDeviceContext = pGraphics->GetDeviceContext();
	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertStride, &pOffsets);
	pDeviceContext->IASetIndexBuffer(mIndexBuffer, mIndexBufferFormat, 0);
	pDeviceContext->DrawIndexed(mIndexCount, 0, 0);
}
