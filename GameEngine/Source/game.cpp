#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "stringUtil.h"
#include "VertexFormat.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "SkinnedObj.h"
#include "Camera.h"
#include "RenderCube.h"
#include "Material.h"
#include "AssetManager.h"
#include "mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "jsonUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include "Components/PointLight.h"
#include "Components/Character.h"
#include <fstream>
#include <sstream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	// Initialize frame borders
	mGraphics.InitD3D(hWnd, width, height);

	// Create camera
	mCamera = new Camera();

	// Initialize Lighting Buffer and upload data
	Graphics* pGraphics = Graphics::Get();
	uint32_t mLightingConstantsSize = sizeof(mLightingConstants);
	mLightingBuffer = pGraphics->CreateGraphicsBuffer(&mLightingConstants, mLightingConstantsSize, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);

	// Add order for rendering vertices
	uint16_t indexData[] = { 0, 1, 2 };

	// Create basic input layout 
	D3D11_INPUT_ELEMENT_DESC basicInputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create mesh input layout 
	D3D11_INPUT_ELEMENT_DESC basicMeshInputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUvNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColorUvNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColorUvNormal, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosColorUvNormal, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create phong-shaded input layout 
	D3D11_INPUT_ELEMENT_DESC phongInputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUvNormal, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosUvNormal, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexPosUvNormal, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create skinned input layout 
	D3D11_INPUT_ELEMENT_DESC skinnedInputElem[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SkinnedInputLayout, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SkinnedInputLayout, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(SkinnedInputLayout, bones), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(SkinnedInputLayout, weights), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(SkinnedInputLayout, uv), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mAssetManager = new AssetManager();

	// Load and upload the simple shader
	Shader* mSimpleShader = new Shader();
	mSimpleShader->Load(L"Shaders/Mesh.hlsl", basicInputElem, ARRAY_SIZE(basicInputElem));
	mAssetManager->SetShader(L"Mesh", mSimpleShader);

	// Load and upload the basic mesh shader
	Shader* mBasicMeshShader = new Shader();
	mBasicMeshShader->Load(L"Shaders/BasicMesh.hlsl", basicMeshInputElem, ARRAY_SIZE(basicMeshInputElem));
	mAssetManager->SetShader(L"BasicMesh", mBasicMeshShader);

	// Load and upload phong shader
	Shader* mPhongShader = new Shader();
	mPhongShader->Load(L"Shaders/Phong.hlsl", phongInputElem, ARRAY_SIZE(phongInputElem));
	mAssetManager->SetShader(L"Phong", mPhongShader);

	// Load and upload unlit shader
	Shader* mUnlitShader = new Shader();
	mUnlitShader->Load(L"Shaders/Unlit.hlsl", phongInputElem, ARRAY_SIZE(phongInputElem));
	mAssetManager->SetShader(L"Unlit", mUnlitShader);

	// Load and upload the skinned shader
	Shader* mSkinnedShader = new Shader();
	mSkinnedShader->Load(L"Shaders/Skinned.hlsl", skinnedInputElem, ARRAY_SIZE(skinnedInputElem));
	mAssetManager->SetShader(L"Skinned", mSkinnedShader);

	// load debug level
	LoadLevel(L"Assets/Levels/Level07.itplevel");

	// DEBUG: Load Skeleton
	Skeleton* mSkeleton = mAssetManager->LoadSkeleton(L"Assets/Anims/SK_Mannequin.itpskel");

	// DEBUG: Load Animation
	Animation* mAnim = mAssetManager->LoadAnimation(L"Assets/Anims/ThirdPersonRun.itpanim2");


}

void Game::Shutdown()
{
	// Delete camera
	delete(mCamera);

	// Delete render objects
	for (RenderObj* renderObj : mRenderedObjects)
	{
		delete(renderObj);
	}

	// release lighting buffer
	mLightingBuffer->Release();

	// release asset manager and all cached objects
	mAssetManager->Clear();

	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	for (RenderObj* obj : mRenderedObjects)
	{
		obj->Update(deltaTime);
	}
}

void Game::RenderFrame()
{
	// Set the render target
	mGraphics.SetRenderTarget(mGraphics.GetBackBuffer(), mGraphics.GetDepthStencilView());

	{	// Clear the screen to blue
		Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
		mGraphics.ClearRenderTarget(clearColor);

		// Clear the depth buffer before redrawing
		mGraphics.ClearDepthBuffer(mGraphics.GetDepthStencilView(), 1.0f);
	}

	// Uploading lighting buffer
	mGraphics.UploadBuffer(mLightingBuffer, &mLightingConstants, sizeof(mLightingConstants));
	mGraphics.GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_LIGHT, 1, &mLightingBuffer);
	
	// Set the main camera active
	mCamera->SetActive();

	// Draw all objects
	for (RenderObj* renderObj : mRenderedObjects)
	{
		renderObj->Draw();
	}

	// End after all draws to the frame are complete
	mGraphics.EndFrame();
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

Lights::PointLightData* Game::AllocateLight()
{
	// go through array of point lights, find one that is not enabled. enable it and return it
	for (int i=0; i<Lights::maxPointLights; i++)
	{
		if (!mLightingConstants.c_pointLight[i].isEnabled)
		{
			mLightingConstants.c_pointLight[i].isEnabled = true;
			return &mLightingConstants.c_pointLight[i];
		}
	}
	return nullptr;
}

void Game::FreeLight(Lights::PointLightData* pLight)
{
	pLight->isEnabled = false;
}

void Game::SetAmbientLight(const Vector3& color)
{
	mLightingConstants.c_ambient = color;
}

const Vector3& Game::GetAmbientLight() const
{
	return mLightingConstants.c_ambient;
}

bool Game::LoadLevel(const WCHAR* fileName)
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
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	// load camera data
	Vector3 cameraPos = {};
	GetVectorFromJSON(doc["camera"], "position", cameraPos);

	Quaternion cameraRot = {};
	GetQuaternionFromJSON(doc["camera"], "rotation", cameraRot);

	Matrix4 rotationMat = Matrix4::CreateFromQuaternion(cameraRot);
	Matrix4 translationMat = Matrix4::CreateTranslation(cameraPos);
	Matrix4 view = translationMat * rotationMat; // camera-to-world
	view.Invert(); // world-to-camera
	mCamera->SetViewMatrix(view);

	// load lighting data
	Vector3 ambientLighting = {};
	GetVectorFromJSON(doc["lightingData"], "ambient", ambientLighting);
	SetAmbientLight(ambientLighting);

	// load render objects
	for (uint32_t i = 0; i < doc["renderObjects"].Size(); i++)
	{
		std::wstring name = L"";
		GetWStringFromJSON(doc["renderObjects"][i], "mesh", name);
		Mesh* newMesh = mAssetManager->LoadMesh(name);

		// create a skinned object instead if it is skinned
		RenderObj* newObj;
		if (newMesh->IsSkinned())
		{
			newObj = new SkinnedObj(newMesh);
		}
		else
		{
			newObj = new RenderObj(newMesh);
		}

		float scale = 0.0f;
		GetFloatFromJSON(doc["renderObjects"][i], "scale", scale);
		Matrix4 scaleMat = Matrix4::CreateScale(scale);

		Vector3 pos = Vector3::Zero;
		GetVectorFromJSON(doc["renderObjects"][i], "position", pos);
		Matrix4 posMat = Matrix4::CreateTranslation(pos);

		Quaternion rot = Quaternion::Identity;
		GetQuaternionFromJSON(doc["renderObjects"][i], "rotation", rot);
		Matrix4 rotMat = Matrix4::CreateFromQuaternion(rot);

		Matrix4 finalTransformMatrix = scaleMat * rotMat * posMat;
		newObj->SetObjectConstants(finalTransformMatrix);

		if (doc["renderObjects"][i].HasMember("components") && doc["renderObjects"][i]["components"].IsArray())
		{
			for (uint32_t j = 0; j < doc["renderObjects"][i]["components"].Size(); j++)
			{
				std::string componentName = "";
				GetStringFromJSON(doc["renderObjects"][i]["components"][j], "type", componentName);
				if (componentName == "PointLight")
				{
					PointLight* pl = new PointLight(this, newObj);
					pl->LoadProperties(doc["renderObjects"][i]["components"][j]);
					newObj->AddComponent(pl);
				}
				else if (componentName == "Character")
				{
					Character* character = new Character(this, newObj);
					character->LoadProperties(doc["renderObjects"][i]["components"][j]);
					newObj->AddComponent(character);
				}
			}
		}
		mRenderedObjects.push_back(newObj);
	}

	return true;
}