#include "stdafx.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "stringUtil.h"
#include "Animation.h"
#include "BoneTransform.h"
#include <iostream>
#include <fstream>
#include <sstream>

Animation::Animation()
{
}

Animation::~Animation()
{
}

uint32_t Animation::GetNumBones() const
{
	return mBoneCount;
}

uint32_t Animation::GetNumFrames() const
{
	return mFrames;
}

float Animation::GetLength() const
{
	return mLength;
}

Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Animation* pAnimation = new Animation();
	if (false == pAnimation->Load(fileName))
	{
		delete pAnimation;
		return new Animation();
	}
	return pAnimation;
}

bool Animation::Load(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
		return false;
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
		return false;
	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();
	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpanim" ||
		ver != 2)
		return false;
	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
		return false;
	const rapidjson::Value& frames = sequence["frames"];
	if (!frames.IsUint())
		return false;
	mFrames = frames.GetUint();
	const rapidjson::Value& length = sequence["length"];
	if (!length.IsDouble())
		return false;
	mLength = length.GetDouble();
	const rapidjson::Value& bonecount = sequence["bonecount"];
	if (!bonecount.IsUint())
		return false;
	mBoneCount = bonecount.GetUint();
	const rapidjson::Value& tracks = sequence["tracks"];
	if (!tracks.IsArray())
		return false;

	mAnimationData.reserve(mBoneCount);
	mAnimationData.resize(mBoneCount);

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		if (!tracks[i]["bone"].IsUint())
			return false;
		uint32_t bone = tracks[i]["bone"].GetUint();

		if (!tracks[i]["transforms"].IsArray())
			return false;
		int numTransforms = tracks[i]["transforms"].Size();
		mAnimationData[bone].resize(numTransforms);

		for (int j = 0; j < numTransforms; j++)
		{
			BoneTransform temp;

			const rapidjson::Value& transform = tracks[i]["transforms"][j];
			if (!transform.IsObject())
				return false;

			if (false == GetQuaternionFromJSON(transform, "rot", temp.mRot))
				return false;
			if (false == GetVectorFromJSON(transform, "trans", temp.mPos))
				return false;

			mAnimationData[bone][j] = temp;
		}

	}
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	outPoses.clear();
	outPoses.resize(mAnimationData.size());

	// choose keyframe closest to current inTime
	float timePerFrame = GetLength() / (float)(GetNumFrames() - 1);
	float keyframe = (inTime / timePerFrame);

	// get two frames to interpolate between
	int frame1 = static_cast<int>(keyframe);
	int frame2 = frame1 + 1;
	frame1 = (std::max)(0, (std::min)(frame1, static_cast<int>(GetNumFrames() - 1)));
	frame2 = (std::max)(0, (std::min)(frame2, static_cast<int>(GetNumFrames() - 1)));

	float f = 0.0f;
	if (frame1 != frame2)
	{
		f = keyframe - frame1;
	}

	for (int i = 0; i < mBoneCount; i++)
	{
		Matrix4 bonePose;
		if (mAnimationData[i].size() == 0)
		{
			// use identity matrix for bone with no animation
			bonePose = Matrix4::Identity;
		}
		else
		{
			// get data for interpolation
			const BoneTransform& a = mAnimationData[i][frame1];
			const BoneTransform& b = mAnimationData[i][frame2];

			// interpolate between the keyframes
			BoneTransform interpolated = BoneTransform::Interpolate(a, b, f);

			bonePose = interpolated.ToMatrix();
		}	

		// combine bone's matrix with parent's matrix if there is one
		int parentIdx = inSkeleton->GetBone(i).mParent;
		if (parentIdx != -1)
		{
			bonePose *= outPoses[parentIdx];
		}

		outPoses[i] = bonePose;
	}
}

