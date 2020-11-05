#include "GameScene.h"
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"

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
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    tileMap_.DrawUpdate();
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

    // 最初に一回スクリーンに描画して記録しておく
    DrawOwnScene();
}
