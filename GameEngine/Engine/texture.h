#pragma once

class AssetManager;
class Texture
{
public:
    Texture();
    ~Texture();
    
    void Free();
    bool Load(const WCHAR* fileName);
    static Texture* StaticLoad(const WCHAR* fileName, AssetManager* pManager);
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }
    void SetActive(int slot) const;

private:
    ID3D11Resource *mResource = nullptr;
    ID3D11ShaderResourceView *mView = nullptr;
    int mWidth = 0;
    int mHeight = 0;
};