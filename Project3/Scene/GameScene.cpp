#include "GameScene.h"
#include <algorithm>
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"
#include "../Player.h"
#include "../Bomb.h"

GameScene::GameScene()
{
    TRACE("ｹﾞｰﾑｼｰﾝの生成\n");
    Init();
    InitFunc();
    now_ = std::chrono::system_clock::now();
}

GameScene::~GameScene()
{
}

uniqueBase GameScene::Update(uniqueBase scene)
{
    stateUpdate_[gameState_]();
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    tileMap_.Update();

    for (auto& obj : objList_)
    {
        obj->Draw();
    }

    end_ = lpSceneMng.GetTime();

    if (std::chrono::duration_cast<std::chrono::seconds>(end_ - now_).count() < 1)
    {
        fpsCount_++;
    }
    else
    {
        now_ = std::chrono::system_clock::now();
        fps_ = fpsCount_;
        fpsCount_ = 0;
        averageCount_++;
    }

    DrawBox(0, 0, 80, 30, 0xf, true);
    DrawFormatString(0, 5, 0xff, "1/%d", fps_);
    DrawBox(100, 0, 250, 30, 0xf, true);
    DrawFormatString(100, 5, 0xff, "LostAverage : %d", Player::lostCont_ / averageCount_);
}

void GameScene::Init(void)
{

    auto mode = lpNetWork.GetNetWorkMode();
    if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
    {
        if (!tileMap_.LoadTmx("TileMap/Stage01.tmx"))
        {
            TRACE("TMXファイルが開けません\n");
            return;
        }
    }
    else if (mode == NetWorkMode::GUEST)
    {
        if (!tileMap_.LoadTmx("TileMap/SendData.tmx"))
        {
            TRACE("TMXファイルが開けません\n");
            return;
        }
    }

    if (mode != NetWorkMode::OFFLINE)
    {
        int max = lpNetWork.GetPlayerMax();
        int playerCnt = 1;
        for (auto& charData : tileMap_.GetCharChipPos())
        {
            if (playerCnt <= max)
            {
                Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
                objList_.emplace_back(std::make_shared<Player>(pos, *this, tileMap_.GetLayerVec(),lpNetWork.GetPlayerID()));
                playerCnt++;
            }
        }
    }
    else
    {
        for (auto& charData : tileMap_.GetCharChipPos())
        {
            Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
            objList_.emplace_back(std::make_shared<Player>(pos, *this, tileMap_.GetLayerVec(), lpNetWork.GetPlayerID()));
        }
    }

    averageCount_ = 0;
    fpsCount_ = 0;
    fps_ = 0;
    // 最初に一回スクリーンに描画して記録しておく
    DrawOwnScene();
}

bool GameScene::SetBomb(int& ownerID, int& myID, Vector2& pos, chronoTime& chronoTime, int length, bool flg)
{
    if (flg)
    {
        // インスタンス情報の送信
        UnionData data[7]{};
        data[0].iData = ownerID;
        data[1].iData = myID;
        data[2].iData = pos.x;
        data[3].iData = pos.y;
        data[4].iData = length;
        TimeData timeData{ chronoTime };
        data[5].iData = timeData.iData[0];
        data[6].iData = timeData.iData[1];

        lpNetWork.SendMes(MesType::SET_BOMB, UnionVec{ data[0],data[1],data[2],data[3],data[4],data[5], data[6] });
    }
 
    objList_.emplace_back(std::make_shared<Bomb>(myID, pos, chronoTime, *this));

    return true;
}

sharedObj GameScene::GetObjectList(int id)
{
    for (auto& list : objList_)
    {
        if (list != nullptr)
        {
            if (list->ID() == id)
            {
                return list;
            }
        }
    }

    sharedObj obj = nullptr;
    return obj;
}

void GameScene::FlameGenerate(int& length, Vector2& pos)
{
    return tileMap_.AddFlameGenerate(length, pos);
}

bool GameScene::CheckHitFlame(int chipPos)
{
    auto check = tileMap_.GetFlameMap()[chipPos].startTime.time_since_epoch().count();
    if (check != 0)
    {
        return true;
    }
    return false;
}

void GameScene::SetBombMap(int chipPos, bool flg)
{
    tileMap_.SetBombMap(chipPos, flg);
}

const std::vector<bool> GameScene::GetBombMap(void)
{
    return tileMap_.GetBombMap();
}

void GameScene::InitFunc()
{
    gameState_ = GameState::Wait;

    auto playUpdate = [&]()
    {
        DrawOwnScene();

        objList_.sort([](sharedObj& obj1, sharedObj& obj2)
            {return obj1->CheckData(MesType::POS) > obj2->CheckData(MesType::POS); });

        for (auto& obj : objList_)
        {
            obj->Update();
        }

        objList_.erase(std::remove_if(objList_.begin(), objList_.end(), [&](sharedObj& obj) {return !obj->Alive(); }), objList_.end());
    };

    auto waitUpdate = [&]()
    {
        DrawOwnScene();
        auto size = lpSceneMng.screenSize_ / 2;
        auto startTime = lpNetWork.GetStartTime();
        if (lpNetWork.GetStartCntFlg())
        {
            chronoTime now = std::chrono::system_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            auto tmp = (COUNT_START_MAX - time) / 1000;
            if (tmp <= 0)
            {
                gameState_ = GameState::Play;
            }
            DrawFormatString(size.x, size.y, 0xffffff, "残り：%d秒", (COUNT_START_MAX - time) / 1000);
        }
    };

    stateUpdate_.try_emplace(GameState::Wait, waitUpdate);
    stateUpdate_.try_emplace(GameState::Play, playUpdate);
}
