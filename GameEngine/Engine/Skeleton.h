#pragma once
#include "engineMath.h"
#include "BoneTransform.h"

class AssetManager;

struct Bone
{
	BoneTransform mBindPose;
	std::string mName;
	int mParent;
};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	size_t GetNumBones() const;
	const Bone& GetBone(size_t idx) const;
	const std::vector<Bone>& GetBones() const;
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const;

	static Skeleton* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
	bool Load(const WCHAR* fileName);

private:
	void ComputeGlobalInvBindPose();

	std::vector<Bone> mBones;
	std::vector<Matrix4> mInvBindPoseMat;
};

