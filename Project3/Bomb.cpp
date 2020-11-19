#include "Bomb.h"
#include <DxLib.h>
#include <cassert>
#include "common/ImageMng.h"
#include "Scene/BaseScene.h"
#include "Scene/GameScene.h"

Bomb::Bomb(int& id, Vector2& pos, BaseScene& scene) :scene_(scene)
{
    pos_ = pos;
    id_ = id;
    pos_ = pos;
    vel_ = { 4,4 };
    rad_ = 0;
}

Bomb::~Bomb()
{
}

bool Bomb::Update(LayerVec&& layer)
{
    if (dethCnt_ > 180)
    {
        alive_ = false;

        try
        {
            dynamic_cast<GameScene&>(scene_).GetObject(id_ / UNIT_ID_BASE);
        }
        catch (...)
        {
            assert(!"シーンのキャスト失敗");
        }
    }
    Draw();
    return true;
}

void Bomb::Draw(void)
{
    VecInt handle = lpImageMng.GetHandle("Image/bomb.png", { 2,7 }, { 32,32 });

    DrawGraph(pos_.x, pos_.y, handle[0], true);
}

bool Bomb::UpdateDef(LayerVec& layer)
{
    return false;
}

bool Bomb::UpdateAuto(LayerVec& layer)
{
    return false;
}

bool Bomb::UpdateRev(LayerVec& layer)
{
    return false;
}
