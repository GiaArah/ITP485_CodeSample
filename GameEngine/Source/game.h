#pragma once
#include "Graphics.h"
#include "Lights.h"

class RenderObj;
class Camera;
class AssetManager;

class Game
{
public:
    Game();
    ~Game();

    void Init(HWND hWnd, float width, float height);
    void Shutdown();
	void Update(float deltaTime);
    void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

	Lights::PointLightData* AllocateLight();
	void FreeLight(Lights::PointLightData* pLight);

	AssetManager* GetAssetManager() { return mAssetManager; }

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;

	Graphics mGraphics;
	AssetManager* mAssetManager = nullptr;
	Camera* mCamera = nullptr;
	std::vector<RenderObj*> mRenderedObjects = {};

	Lights::LightingConstants mLightingConstants = {};
	ID3D11Buffer* mLightingBuffer;

	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight() const;

	bool LoadLevel(const WCHAR* fileName);
};