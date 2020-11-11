#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/TileMap.h"

class Player;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    uniqueBase Update(uniqueBase scene)override;
    void DrawOwnScene(void)override;
    void Init(void)override;
private:

    TileMap tileMap_;                                   // タイルマップ情報格納

    std::shared_ptr<Player> player_;                    // プレイヤー
};

