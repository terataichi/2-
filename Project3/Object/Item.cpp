#include "Item.h"
#include "../common/ImageMng.h"
#include "../Scene/GameScene.h"

Item::Item(int& id ,ItemType type, Vector2& pos, BaseScene& baseScene):scene_(baseScene)
{
    itemType_ = type;
    pos_ = pos;
    state_ = STATE::Non;
    moveCnt_ = 0;
    InitFunc();
    sinPosY_ = 0;
    zOrder_ = 2;
    id_ = id;
}

Item::~Item()
{
}

bool Item::Update(void)
{
    if (!alive_)
    {
        return false;
    }

    stateUpdate_[state_]();
    return true;
}

void Item::Draw(void)
{
    auto image = lpImageMng.GetHandle("Image/map.png", { 4,3 }, { 32,32 })[8 + static_cast<int>(itemType_)];
    DrawGraph(pos_.x, pos_.y - sinPosY_, image, true);
}

bool Item::UpdateDef()
{
    return false;
}

void Item::InitFunc(void)
{
    auto non = [&]() 
    {
        // ブロックが壊れているか　＆　炎がなくなっているか
        int chipPos = (pos_.y / 32) * 21 + (pos_.x / 32);
        auto flame = dynamic_cast<GameScene&>(scene_).CheckHitFlame(chipPos);
        auto wall = dynamic_cast<GameScene&>(scene_).CheckBlock(chipPos);

        if (!flame && !wall)
        {
            state_ = STATE::Run;
        }
    };

    auto run = [&]() 
    {
        int chipPos = (pos_.y / 32) * 21 + (pos_.x / 32);

        if (dynamic_cast<GameScene&>(scene_).CheckHitFlame(chipPos))
        {

            UnionData data[1]{};
            data[0].iData = id_;

            lpNetWork.SendMesAll(MesType::DETH, UnionVec{ data[0] }, 0);
            alive_ = false;
        }

        moveCnt_++;
        //sinPosY_ = static_cast<int>(pos_.y - 5.0f * cosf((moveCnt_ * 30.0f) * 0.0005f));			// サインカーブ
    };

    stateUpdate_.try_emplace(STATE::Non, non);
    stateUpdate_.try_emplace(STATE::Run, run);

}
