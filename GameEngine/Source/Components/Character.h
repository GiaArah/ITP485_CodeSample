#pragma once
#include "Component.h"

class SkinnedObj; 
class Skeleton;
class Animation;
class Game;

class Character :
    public Component
{
public:
    Character(Game* game, RenderObj* pObj);
    ~Character();

    void UpdateAnim(float deltaTime);
    virtual void Update(float deltaTime) override;

    virtual void LoadProperties(const rapidjson::Value& properties) override;
    bool SetAnim(const std::string& animName);

private:
    SkinnedObj* mSkinnedObj;
    Skeleton* mSkeleton;
    std::unordered_map<std::string, const Animation*> mAnimationTable;

    const Animation* mCurrentAnim;
    float mAnimationTimer = 0.0f;

    Game* mGame;
};

