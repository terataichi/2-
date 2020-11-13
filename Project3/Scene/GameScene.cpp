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
    }
    else if (mode == NetWorkMode::GUEST)
    {
        if (!tileMap_.LoadTmx("TileMap/SendData.tmx"))
        {
            TRACE("TMXファイルが開けません\n");
            return;
        }
    }

    int id = 0;
    for (auto charData : tileMap_.GetCharChipPos())
    {
        Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
        player_.emplace_back(std::make_shared<Player>(id,pos));
        id++;
    }

    // 最初に一回スクリーンに描画して記録しておく
    DrawOwnScene();
}
