#include "GameScene.h"
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"
#include "../Player.h"

GameScene::GameScene()
{
    TRACE("ｹﾞｰﾑｼｰﾝの生成\n");
    Init();
}

GameScene::~GameScene()
{
}

uniqueBase GameScene::Update(uniqueBase scene)
{
    DrawOwnScene();

    for (auto player : player_)
    {
        player->Update(tileMap_.GetLayerData());
    }
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    tileMap_.DrawUpdate();

    for (auto player : player_)
    {
        player->Draw();
    }
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

        if (mode == NetWorkMode::HOST)
        {
            // インスタンス情報の送信
            int id = 0;
            for (auto charData : tileMap_.GetCharChipPos())
            {
                Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };

                // ゲストに情報を送信
                UnionVec unionVec;
                UnionData data;

               data.iData = id;
               unionVec.emplace_back(data);
               data.iData = pos.x;
               unionVec.emplace_back(data);
               data.iData = pos.y;
               unionVec.emplace_back(data);


                lpNetWork.SendMes(MesType::INSTANCE, unionVec);

                player_.emplace_back(std::make_shared<Player>(id,pos));
                id++;
            }

        }
    }
    else if (mode == NetWorkMode::GUEST)
    {
        if (!tileMap_.LoadTmx("TileMap/SendData.tmx"))
        {
            TRACE("TMXファイルが開けません\n");
            return;
        }

        // プレイヤー初期化情報受け取り待ち
        while (!lpNetWork.CheckMes(MesType::INSTANCE))
        {

        }

        UnionVec vec;
        while (lpNetWork.PickRevData(MesType::INSTANCE, vec))
        {
            Vector2 pos{ vec[1].iData,vec[2].iData };
           player_.emplace_back(std::make_shared<Player>(vec[0].iData, pos));
        }
    }

    // 最初に一回スクリーンに描画して記録しておく
    DrawOwnScene();
}
