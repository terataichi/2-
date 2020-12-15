#include "GameScene.h"
#include <algorithm>
#include <DxLib.h>
#include "CrossOverScene.h"
#include "ResultScene.h"
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"
#include "../Object/Player.h"
#include "../Object/Bomb.h"
#include "../Object/Item.h"

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
    if (resultData_.size())
    {
        return std::make_unique<CrossOverScene>(std::move(scene), std::make_unique<ResultScene>(resultData_));
    }
    stateUpdate_[gameState_]();
    tileMap_.Update();
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);

    tileMap_.DrawMap(tileMap_.GetLayerData("Bg"));

    // z順にソート
    objList_.sort([](sharedObj& obj1, sharedObj& obj2)
        {return obj1->zOrder() > obj2->zOrder(); });
    for (auto& obj : objList_)
    {
        if (obj->ObjType() != ObjectType::Player)
        {
            if (obj->Alive())
            {
                obj->Draw();
            }
        }
    }

    tileMap_.DrawMap(tileMap_.GetLayerData("Obj"));

    for (auto& obj : objList_)
    {
        if (obj->ObjType() == ObjectType::Player)
        {
            if (obj->Alive())
            {
                obj->Draw();
            }
        }
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
    DrawFormatString(100, 5, 0xff, "LostAverage : %d", Player::lostCnt_ / averageCount_);
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
        int playerCnt = 0;
        for (auto& charData : tileMap_.GetCharChipPos())
        {
            if (playerCnt <= max - 1)
            {
                Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
                objList_.emplace_back(std::make_shared<Player>(pos, *this, tileMap_.GetLayerVec(),playerCnt * 5));
                playerCnt++;
            }
        }
    }
    else
    {
        int playerCnt = 0;
        for (auto& charData : tileMap_.GetCharChipPos())
        {
            Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
            objList_.emplace_back(std::make_shared<Player>(pos, *this, tileMap_.GetLayerVec(), playerCnt * 5));
            playerCnt++;
        }
    }

    // アイテムの生成
    auto chipData = tileMap_.GetLayerData("Item").chipData;
    Vector2 div{ 32,32 };

    auto posVec = tileMap_.GetItemChipPos();
    int posCnt = 0;
    playerCnt_ = lpNetWork.GetPlayerMax();
    for (auto& chip : chipData)
    {
        if (chip != 0)
        {
            Vector2 pos{ posVec[posCnt].x * tileMap_.GetMapData().tileWidth, posVec[posCnt].y * tileMap_.GetMapData().tileHeight };
            objList_.emplace_back(std::make_shared<Item>(static_cast<ItemType>(chip % 9), pos, *this));
            posCnt++;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        deathList_.emplace_front(-1);
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
 
    objList_.emplace_back(std::make_shared<Bomb>(myID, pos, chronoTime,length, *this));

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

bool GameScene::CheckBlock(int chipPos)
{
    return tileMap_.GetLayerData("Obj").chipData[chipPos];
}

int GameScene::CheckHitItem(int chipPos)
{
    for (auto& list : objList_)
    {
        if (list->ObjType() == ObjectType::Item)
        {
            int itemPos = (list->pos().y / 32) * 21 + (list->pos().x / 32);
            if (itemPos == chipPos)
            {
                list->SetAlive(false);
                
                auto type = dynamic_cast<Item&>(*list).GetItemType();
                return static_cast<int>(type);
            }
        }
    }

    return -1;
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
    gameState_ = GameState::Play;
    if (lpNetWork.GetNetWorkMode() != NetWorkMode::OFFLINE)
    {
        gameState_ = GameState::Wait;
    }

    auto playUpdate = [&]()
    {
        DrawOwnScene();

        objList_.sort([](sharedObj& obj1, sharedObj& obj2)
            {return obj1->CheckData(MesType::POS) > obj2->CheckData(MesType::POS); });

        for (auto& obj : objList_)
        {
            if (obj->Alive())
            {
                obj->Update();
                if (!obj->Alive() && obj->ObjType() == ObjectType::Player)
                {
                    deathList_.emplace_front(obj->ID());
                    playerCnt_--;
                }
            }
        }
        if (lpNetWork.GetNetWorkMode() == NetWorkMode::HOST)
        {
            auto flg = true;
            for (auto& flame : tileMap_.GetFlameMap())
            {
                flg &= (flame.startTime.time_since_epoch().count() == 0);
            }

            if (playerCnt_ <= 1 && flg)
            {
                for (auto& obj : objList_)
                {
                    if (obj->Alive())
                    {
                        if (obj->ObjType() == ObjectType::Player)
                        {
                            deathList_.emplace_front(obj->ID());
                        }
                    }
                }
                UnionData data[5]{};
                int i = 0;
                for (auto list : deathList_)
                {
                    data[i].iData = list;
                    i++;
                    if (i >= 5)
                    {
                        break;
                    }
                }
               
                resultData_ = UnionVec{ data[0],data[1],data[2],data[3],data[4] };
                lpNetWork.SendMesAll(MesType::RESULT, resultData_, -1);
                return false;
            }
        }
        if (lpNetWork.GetNetWorkMode() == NetWorkMode::GUEST)
        {
            resultData_ = lpNetWork.GetResultData();
        }

        // 死んでるやつ削除
        objList_.erase(std::remove_if(objList_.begin(), objList_.end(), [&](sharedObj& obj) 
            {return !obj->Alive() && (obj->ObjType() != ObjectType::Player); }), objList_.end());
        return true;

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
        return true;
    };

    stateUpdate_.try_emplace(GameState::Wait, waitUpdate);
    stateUpdate_.try_emplace(GameState::Play, playUpdate);
}
