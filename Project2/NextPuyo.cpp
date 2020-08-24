#include "NextPuyo.h"
#include <random>
#include <DxLib.h>
#include "Scene/SceneMng.h"

int NextPuyo::count_ = 0;

NextPuyo::NextPuyo(Vector2& pos,int drawCnt)
{
    screenID_ = MakeScreen(97, 129,true);
    nextPuyoList_.clear();
    pos_ = pos;
    drawCnt_ = drawCnt;
    id_ = count_;
    count_++;
    Add(2);
}

NextPuyo::~NextPuyo()
{
}

pairPuyo NextPuyo::PickUp()
{
    auto nextdata = *nextPuyoList_.begin();         // リストの中のpairにアクセスしたいので*を付ける
    nextPuyoList_.erase(nextPuyoList_.begin());     // コピーはとったので削除
    Add(1);                                         // 消したので新しくインスタンス
    return nextdata;
}

int NextPuyo::screenID()
{
    return screenID_;
}

void NextPuyo::Draw()
{
    auto count = drawCnt_;
    auto id = GetDrawScreen();                      // 描画先画面を取得する
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    // カウントの数だけネクストを描画する
    for (auto puyo : nextPuyoList_)
    {
        puyo.first->PuyonUpdate();
        puyo.first->Draw();
        puyo.second->PuyonUpdate();
        puyo.second->Draw();
        count--;
        if (count <= 0)
        {
            break;
        }
    }

    SetDrawScreen(id);
    //DrawGraph(pos_.x, pos_.y, screenID_, true);

    lpSceneMng.AddDrawQue({ screenID_,{pos_.x + 48, pos_.y + 48},0.0, 1 });
}

bool NextPuyo::Add(int no)
{
    // ランダムの生成
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> puyoRand(static_cast<int>(PuyoID::Red), static_cast<int>(PuyoID::Purple));

    // もらってきた数だけインスタンスする
    while (no > 0)
    {
        nextPuyoList_.push_back
        (
            {
                std::make_shared<puyo>(Vector2{0,0},static_cast<PuyoID>(puyoRand(mt))),
                std::make_shared<puyo>(Vector2{0,64},static_cast<PuyoID>(puyoRand(mt))),
            }
        );
        no--;
    }


    Vector2 pos1{ 32,32 };
    Vector2 pos2{ 32,96 };
    Vector2 offset{ 64,32 };
    if (id_ % 2 == 1)
    {
        pos1 = { 64,32 };
        pos2 = { 64,96 };
        offset = { -64,32 };
    }

    for (auto puyoPair : nextPuyoList_)
    {
        puyoPair.first->pos(pos1);
        puyoPair.second->pos(pos2);
        pos1 += offset;
        pos2 += offset;
    }

    return false;
}
