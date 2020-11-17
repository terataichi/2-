#include "GameScene.h"
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"
#include "../Player.h"

GameScene::GameScene()
{
    TRACE("ｹﾞｰﾑｼｰﾝの生成\n");
    Init();
    now_ = std::chrono::system_clock::now();
}

GameScene::~GameScene()
{
}

uniqueBase GameScene::Update(uniqueBase scene)
{
    DrawOwnScene();

    for (auto& player : player_)
    {
        player->Update(tileMap_.GetLayerData());
    }
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    tileMap_.DrawUpdate();

    for (auto& player : player_)
    {
        player->Draw();
    }

    end_ = std::chrono::system_clock::now();

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

    for (auto charData : tileMap_.GetCharChipPos())
    {
        Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
        player_.emplace_back(std::make_shared<Player>(pos));
    }

    averageCount_ = 0;
    fpsCount_ = 0;
    fps_ = 0;
    // 最初に一回スクリーンに描画して記録しておく
    DrawOwnScene();
}
