#include "GameScene.h"
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"
#include "../NetWork/NetWork.h"
#include "../Player.h"

GameScene::GameScene()
{
    TRACE("�ްѼ�݂̐���\n");
    Init();
}

GameScene::~GameScene()
{
}

uniqueBase GameScene::Update(uniqueBase scene)
{
    DrawOwnScene();
    player_->Update(tileMap_.GetLayerData());
    return scene;
}

void GameScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    tileMap_.DrawUpdate();
    player_->Draw();
}

void GameScene::Init(void)
{

    auto mode = lpNetWork.GetNetWorkMode();
    if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
    {
        if (!tileMap_.LoadTmx("TileMap/Stage01.tmx"))
        {
            TRACE("TMX�t�@�C�����J���܂���\n");
            return;
        }

        if (mode == NetWorkMode::HOST)
        {
            // �C���X�^���X���̑��M
        }
    }
    else if (mode == NetWorkMode::GUEST)
    {
        if (!tileMap_.LoadTmx("TileMap/SendData.tmx"))
        {
            TRACE("TMX�t�@�C�����J���܂���\n");
            return;
        }
    }


    //for (auto charData : tileMap_.GetCharChipPos())
    {
        auto charData = tileMap_.GetCharChipPos()[0];

        Vector2 pos{ charData.x * tileMap_.GetMapData().tileWidth,charData.y * tileMap_.GetMapData().tileHeight };
        player_ = std::make_shared<Player>(pos);
    }


    // �ŏ��Ɉ��X�N���[���ɕ`�悵�ċL�^���Ă���
    DrawOwnScene();
}
