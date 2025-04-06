#pragma once

class BoneTransform; 
class AssetManager;

class Animation
{
public:
	Animation();
	~Animation();

	uint32_t GetNumBones() const;
	uint32_t GetNumFrames() const;
	float GetLength() const;

	static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
	bool Load(const WCHAR* fileName);

	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const;

private:
	uint32_t mBoneCount;
	uint32_t mFrames;
	float mLength;
	std::vector<std::vector<BoneTransform>> mAnimationData;
};

