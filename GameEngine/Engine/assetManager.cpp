#include "stdafx.h"
#include "assetManager.h"
#include "assetCache.h"

AssetManager::AssetManager() :
	mShaderCache(this),
	mTextureCache(this),
	mMaterialCache(this),
	mMeshCache(this),
	mSkeletonCache(this),
	mAnimationCache(this)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::Clear()
{
	mShaderCache.Clear();
	mTextureCache.Clear();
	mMaterialCache.Clear();
	mMeshCache.Clear();
	mSkeletonCache.Clear();
	mAnimationCache.Clear();
}

Shader* AssetManager::GetShader(const std::wstring& shaderName)
{
	return mShaderCache.Get(shaderName);
}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader)
{
	mShaderCache.Cache(shaderName, pShader);
}

Texture* AssetManager::LoadTexture(const std::wstring& fileName)
{
	return mTextureCache.Load(fileName);
}

Material* AssetManager::LoadMaterial(const std::wstring& fileName)
{
	return mMaterialCache.Load(fileName);
}

Mesh* AssetManager::LoadMesh(const std::wstring& fileName)
{
	return mMeshCache.Load(fileName);
}

Skeleton* AssetManager::LoadSkeleton(const std::wstring& fileName)
{
	return mSkeletonCache.Load(fileName);
}

Animation* AssetManager::LoadAnimation(const std::wstring& fileName)
{
	return mAnimationCache.Load(fileName);
}
