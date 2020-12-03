#include "Item.h"
#include "../common/ImageMng.h"

Item::Item(ItemType type, Vector2& pos)
{
    itemType_ = type;
    pos_ = pos;
    state_ = STATE::Non;
    moveCnt_ = 0;
}

Item::~Item()
{
}

bool Item::Update(void)
{
    stateUpdate_[state_]();
    return false;
}

void Item::Draw(void)
{
    auto image = lpImageMng.GetHandle("Image/map.png", { 4,3 }, { 32,32 })[8 + static_cast<int>(itemType_)];
    DrawGraph(pos_.x, pos_.y + sinPosY_, image, true);
}

void Item::InitFunc(void)
{
    auto non = [&]() 
    {
        {
            state_ = STATE::Run;
        }
    };

    
    auto run = [&]() 
    {
        moveCnt_++;
        sinPosY_ = static_cast<int>(pos_.y - 10.0f * cosf((moveCnt_ * 30.0f) * 0.015f));			// サインカーブ
    };

    stateUpdate_.try_emplace(STATE::Non, non);
    stateUpdate_.try_emplace(STATE::Run, run);

}
