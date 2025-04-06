#include "stdafx.h"
#include "Material.h"
#include "Shader.h"
#include "texture.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "stringUtil.h"
#include <iostream>
#include <fstream>
#include <sstream>

Material::Material()
{
	// bind buffer to constant buffer on pixel shader
	uint32_t materialDataSize = sizeof(mConst);
	Graphics* pGraphics = Graphics::Get();
	mConstantBuffer = pGraphics->CreateGraphicsBuffer(&mConst, materialDataSize, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

Material::~Material()
{
	mConstantBuffer->Release();
}

void Material::SetActive()
{
	// set shader active
	mShader->SetActive();

	// loop through all textures and set each active on its matching slot
	for (int i=0; i<ARRAY_SIZE(mTexture); i++)
	{
		if (mTexture[i] != nullptr)
		{
			mTexture[i]->SetActive(i);
		}
	}

	// Upload the object data to the buffers, set the buffers, and draw
	Graphics* pGraphics = Graphics::Get();
	pGraphics->UploadBuffer(mConstantBuffer, &mConst, sizeof(mConst));

	ID3D11DeviceContext* pDeviceContext = pGraphics->GetDeviceContext();
	pDeviceContext->VSSetConstantBuffers(2, 1, &mConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(2, 1, &mConstantBuffer);

}

void Material::SetShader(const Shader* shader)
{
	mShader = shader;
}

void Material::SetTexture(int slot, const Texture* texture)
{
	mTexture[slot] = texture;
}

void Material::SetDiffuseColor(const Vector3& color)
{
	mConst.diffuseColor = color;
}

void Material::SetSpecularColor(const Vector3& color)
{
	mConst.specularColor = color;
}

void Material::SetSpecularPower(float power)
{
	mConst.specularPower = power;
}

Material* Material::StaticLoad(const WCHAR* fileName, AssetManager* pManager)
{
	Material* pMat = new Material();
	if (false == pMat->Load(fileName, pManager))
	{
		delete pMat;
		return new Material();
	}
	return pMat;
}

bool Material::Load(const WCHAR* fileName, AssetManager* pAssetManager)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		return false;
	}
	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();
	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpmat" ||
		ver != 1)
	{
		return false;
	}
	// Load Shader
	std::wstring shaderName;
	if (false == GetWStringFromJSON(doc, "shader", shaderName))
		return false;
	mShader = pAssetManager->GetShader(shaderName);
	DbgAssert(nullptr != mShader, "Material unable to load shader");
	{ // Load textures
		const rapidjson::Value& textures = doc["textures"];
		if (textures.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
			{
				if (i < Graphics::TEXTURE_SLOT_TOTAL)
				{
					std::wstring textureName;
					StringUtil::String2WString(textureName,
						textures[i].GetString());
					mTexture[i] = pAssetManager->LoadTexture(textureName);
				}
			}
		}
	}
	// Load the Lighting Parameters
	GetVectorFromJSON(doc, "diffuseColor", mConst.diffuseColor);
	GetVectorFromJSON(doc, "specularColor", mConst.specularColor);
	GetFloatFromJSON(doc, "specularPower", mConst.specularPower);
	return true;
}
