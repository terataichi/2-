#include "Bomb.h"
#include <DxLib.h>
#include <cassert>
#include "common/ImageMng.h"
#include "Scene/BaseScene.h"
#include "Scene/GameScene.h"
#include "Player.h"
#include "FlameGenerator.h"

Bomb::Bomb(int& id, Vector2& pos, chronoTime& time, BaseScene& scene) :scene_(scene)
{
    pos_ = pos;
    id_ = id;
    pos_ = pos;
    vel_ = { 4,4 };
    rad_ = 0;
    dethCnt_ = 0;
    startTime_ = time;
    dynamic_cast<GameScene&>(scene_).SetBombMap((pos_.y / 32) * 21 + (pos_.x / 32), true);
    AnimStateInit();
}

Bomb::~Bomb()
{
}

bool Bomb::Update()
{
    chronoTime now = std::chrono::system_clock::now();
    int chipPos = (pos_.y / 32) * 21 + (pos_.x / 32);
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count() > DETH_CNT_MAX ||
        dynamic_cast<GameScene&>(scene_).CheckHitFlame(chipPos))
    {
        auto object = dynamic_cast<GameScene&>(scene_).GetObjectList((id_ / UNIT_ID_BASE) * UNIT_ID_BASE);

        if (object != nullptr)
        {
            dynamic_cast<Player&>(*object).AddBombList(id_);
        }
 
        if (state_ == STATE::Non)
        {
            state_ = STATE::Deth;
            alive_ = false;
            int tmplength = 3;
            dynamic_cast<GameScene&>(scene_).FlameGenerate(tmplength, pos_);
            dynamic_cast<GameScene&>(scene_).SetBombMap(chipPos, false);
        }
    }
    Draw();
    dethCnt_++;

    return true;
}

void Bomb::Draw(void)
{
    VecInt handle = lpImageMng.GetHandle("Image/bomb.png", { 2,7 }, { 32,32 });

    auto tmpCount = animCnt_ / 60;
    DrawGraph(pos_.x, pos_.y, handle[(tmpCount % 2) * 2], true);
    animCnt_++;
}

bool Bomb::UpdateDef()
{
    return false;
}

bool Bomb::UpdateAuto()
{
    return false;
}

bool Bomb::UpdateRev()
{
    return false;
}

void Bomb::AnimStateInit()
{
    VecInt handle = lpImageMng.GetHandle("Image/bomb.png", { 2,7 }, { 32,32 });

    animState_.try_emplace(STATE::Run, 0, 1);

}
