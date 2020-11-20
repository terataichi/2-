#include "Bomb.h"
#include <DxLib.h>
#include <cassert>
#include "common/ImageMng.h"
#include "Scene/BaseScene.h"
#include "Scene/GameScene.h"
#include "Player.h"

Bomb::Bomb(int& id, Vector2& pos, chronoTime& time, BaseScene& scene) :scene_(scene)
{
    pos_ = pos;
    id_ = id;
    pos_ = pos;
    vel_ = { 4,4 };
    rad_ = 0;
    dethCnt_ = 0;
    startTime_ = time;
}

Bomb::~Bomb()
{
}

bool Bomb::Update()
{
    chronoTime now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_).count() > 3000)
    {
        try
        {
            auto& object = dynamic_cast<GameScene&>(scene_).GetObject((id_ / UNIT_ID_BASE) * UNIT_ID_BASE);
            dynamic_cast<Player&>(object).AddBombList(id_);
        }
        catch (...)
        {
            assert(!"シーンのキャスト失敗");
        }
        alive_ = false;
    }
    Draw();
    dethCnt_++;
    return true;
}

void Bomb::Draw(void)
{
    VecInt handle = lpImageMng.GetHandle("Image/bomb.png", { 2,7 }, { 32,32 });

    DrawGraph(pos_.x, pos_.y, handle[0], true);
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
