#pragma once
#include "EngineMath.h"

class VertexBuffer;
class Material;
class Mesh;
class Component;

struct PerObjectConstants
{
	Matrix4 c_modelToWorld;
};

class RenderObj
{

public:
	RenderObj(const Mesh* mesh);
	virtual ~RenderObj();
	virtual void Draw();
	virtual void Update(float deltaTime);

	void AddComponent(Component* pComp);
	void SetObjectConstants(Matrix4 in) { mObjectConstants.c_modelToWorld = in; }
	PerObjectConstants GetObjectConstants() { return mObjectConstants; }

protected:
	const Mesh* mMesh = nullptr;
	ID3D11Buffer* mObjectBuffer = nullptr;

private:
	PerObjectConstants mObjectConstants = {};
	std::vector<Component*> mComponents = {};
};

