#include "stdafx.h"
#include "Character.h"
#include "RenderObj.h"
#include "SkinnedObj.h"
#include "Skeleton.h"
#include "Animation.h"
#include "game.h"
#include "assetManager.h"
#include "stringUtil.h"
#include "jsonUtil.h"
#include "Animation.h"

Character::Character(Game* game, RenderObj* pObj) : Component(pObj)
{
	mGame = game;
	mSkinnedObj = (SkinnedObj*)pObj;
}

Character::~Character()
{
}

void Character::UpdateAnim(float deltaTime)
{
	mAnimationTimer += deltaTime;
	if (mAnimationTimer > mCurrentAnim->GetLength())
	{
		// wrap the animation time if exceeds anim length
		mAnimationTimer = fmod(mAnimationTimer, mCurrentAnim->GetLength());
	}

	std::vector<Matrix4> outPoses;
	mCurrentAnim->GetGlobalPoseAtTime(outPoses, mSkeleton, mAnimationTimer);

	// combine resulting matrices with the inverse bind pose matrices
	std::vector<Matrix4> skinningMatrices(outPoses.size());
	for (int i = 0; i < outPoses.size(); i++)
	{
		skinningMatrices[i] = mSkeleton->GetGlobalInvBindPoses()[i] * outPoses[i];
	}

	// copy the combined matrices into the skinning matrix buffer
	mSkinnedObj->UploadBufferData(skinningMatrices);
}

void Character::Update(float deltaTime)
{
	if (mCurrentAnim == nullptr)
	{
		SetAnim("run");
	}

	UpdateAnim(deltaTime);
}

void Character::LoadProperties(const rapidjson::Value& properties)
{
	// load skeleton from asset manager "skeleton"
	// access asset manager: get the renderobj this component is attached to, then ___ ????
	std::string skeleton_name = properties["skeleton"].GetString();
	std::wstring sk_name = L"";
	StringUtil::String2WString(sk_name, skeleton_name);
	mSkeleton = mGame->GetAssetManager()->LoadSkeleton(sk_name);

	// fill animation table
	const rapidjson::Value& animations = properties["animations"];
	if (animations.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < animations.Size(); i++)
		{
			// load each animation and enter it into the map
			std::string anim_name = animations[i][0].GetString();
			std::string anim_path = animations[i][1].GetString();
			std::wstring anim_path_w = L"";
			StringUtil::String2WString(anim_path_w, anim_path);

			Animation* new_anim = mGame->GetAssetManager()->LoadAnimation(anim_path_w);
			mAnimationTable.insert(std::pair<std::string, const Animation*>(anim_name, new_anim));
		}
	}

}

bool Character::SetAnim(const std::string& animName)
{
	
	if (mAnimationTable.find(animName) != mAnimationTable.end())
	{
		mCurrentAnim = mAnimationTable[animName];
		mAnimationTimer = 0.0f;
		return true;
	}
	else
	{
		return false;
	}
}
