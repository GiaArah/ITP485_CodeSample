#pragma once
class VertexBuffer
{
public:
	VertexBuffer(
		const void* vertexData, uint32_t vertexCount, uint32_t vertexStride, 
		const void* indexData, uint32_t indexCount, uint32_t indexStride
	);
	~VertexBuffer();
	void Draw() const;

private:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;
	DXGI_FORMAT mIndexBufferFormat = DXGI_FORMAT_R32_UINT;

	uint32_t mVertStride = 0;
	uint32_t mIndexCount = 0;
};

