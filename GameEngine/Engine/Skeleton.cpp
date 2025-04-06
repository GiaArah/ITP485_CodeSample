#include "stdafx.h"
#include "Skeleton.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "stringUtil.h"
#include "BoneTransform.h"
#include <iostream>
#include <fstream>
#include <sstream>

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
}

size_t Skeleton::GetNumBones() const
{
	return mBones.size();
}

const Bone& Skeleton::GetBone(size_t idx) const
{
	return mBones[idx];
}

const std::vector<Bone>& Skeleton::GetBones() const
{
	return mBones;
}

const std::vector<Matrix4>& Skeleton::GetGlobalInvBindPoses() const
{
	return mInvBindPoseMat;
}

Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Skeleton* pSkeleton = new Skeleton();
	if (false == pSkeleton->Load(fileName))
	{
		delete pSkeleton;
		return new Skeleton();
	}
	return pSkeleton;
}

bool Skeleton::Load(const WCHAR* fileName)
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
		str != "itpskel" ||
		ver != 1)
		return false;
	const rapidjson::Value& bonecount = doc["bonecount"];
	if (!bonecount.IsUint())
		return false;
	size_t count = bonecount.GetUint();
	mBones.reserve(count);
	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
		return false;
	if (bones.Size() != count)
		return false;
	Bone temp;
	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
			return false;
		const rapidjson::Value& name = bones[i]["name"];
		temp.mName = name.GetString();
		const rapidjson::Value& parent = bones[i]["parent"];
		temp.mParent = parent.GetInt();
		const rapidjson::Value& bindpose = bones[i]["bindpose"];
		if (!bindpose.IsObject())
			return false;
		if (false == GetQuaternionFromJSON(bindpose, "rot", temp.mBindPose.mRot))
			return false;
		if (false == GetVectorFromJSON(bindpose, "trans", temp.mBindPose.mPos))
			return false;
		mBones.push_back(temp);
	}
	// Now that we have the bones
	ComputeGlobalInvBindPose();
	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	mInvBindPoseMat.resize(mBones.size());

	// loop through all the bones calculating the bind pose of each
	for (int i = 0; i < mBones.size(); i++)
	{
		if (mBones[i].mParent == -1)
		{
			// store the first bone bind pose 
			Matrix4 globalBindPose = mBones[i].mBindPose.ToMatrix();
			mInvBindPoseMat[i] = globalBindPose;
		}
		else
		{
			// compute the bone-to-parent matrix in the bind pose
			// multiply that by the parent's bind pose matrix
			int parentIndex = mBones[i].mParent;
			Matrix4 parentGlobalBindPose = mInvBindPoseMat[parentIndex];
			Matrix4 globalBindPose = mBones[i].mBindPose.ToMatrix() * parentGlobalBindPose;

			// store the newly computed bind pose matrix
			mInvBindPoseMat[i] = globalBindPose;
		}
	}

	// invert all bind pose matrices
	for (int i = 0; i < mInvBindPoseMat.size(); i++)
	{
		mInvBindPoseMat[i].Invert();
	}
}
