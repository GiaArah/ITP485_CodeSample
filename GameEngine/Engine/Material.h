#pragma once
#include "engineMath.h"
#include "Graphics.h"

class Shader;
class Texture;
class AssetManager;

struct MaterialConstants
{
	Vector3 diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	float pad1;
	Vector3 specularColor = Vector3(1.0f, 1.0f, 1.0f);
	float specularPower = 1.0f;
};

class Material
{
public:
	Material();
	~Material();

	void SetActive();
	void SetShader(const Shader* shader);
	void SetTexture(int slot, const Texture* texture);
	void SetDiffuseColor(const Vector3& color);
	void SetSpecularColor(const Vector3& color);
	void SetSpecularPower(float power);

	MaterialConstants GetMaterialConstants() { return mConst; }

	static Material* StaticLoad(const WCHAR* fileName, AssetManager* pManager);
	bool Load(const WCHAR* fileName, AssetManager* pAssetManager);

private:
	MaterialConstants mConst = {};
	ID3D11Buffer* mConstantBuffer = nullptr;
	const Shader* mShader = nullptr;
	const Texture* mTexture[Graphics::TEXTURE_SLOT_TOTAL];
};

