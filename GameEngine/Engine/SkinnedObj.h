#pragma once
#include "RenderObj.h"


struct SkinConstants
{
    Matrix4 c_skinMatrix[80];
};

class SkinnedObj :
    public RenderObj
{
public:
    SkinnedObj(const Mesh* mesh);
    ~SkinnedObj();

    virtual void Draw() override;

    int maxMatrices = 80;
    SkinConstants mSkinConstants = {};

    void UploadBufferData(std::vector<Matrix4>& bonePoseData);
        
private:
    ID3D11Buffer* mSkinnedBuffer = nullptr;

};

